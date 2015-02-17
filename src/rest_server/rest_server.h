// This file is part of MicroRestD <http://github.com/ufal/microrestd/>.
//
// Copyright 2015 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <mutex>
#include <string>

#include "rest_request.h"
#include "rest_service.h"

namespace ufal {
namespace microrestd {

// Forward declarations to void including microhttpd header.
namespace libmicrohttpd {
struct MHD_Daemon;
struct MHD_Connection;
}

class rest_server {
 public:
  rest_server();

  bool set_log_file(const std::string& file_name, unsigned max_log_size);
  void set_min_generated(unsigned min_generated);
  void set_max_connections(unsigned max_connections);
  void set_max_post_size(unsigned max_post_size);
  void set_timeout(unsigned timeout);

  bool start(rest_service* service, unsigned port);
  void wait_until_closed();

 private:
  class microhttpd_request;

  static int handle_request(void* cls, libmicrohttpd::MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls);
  static void request_completed(void* cls, libmicrohttpd::MHD_Connection* connection, void** con_cls, int toe);

  void logf(const char* message, ...);
  void log_append_pair(std::string& message, const char* key, const std::string& value);
  void log_request(const microhttpd_request* request);

  void wait_indefinitely();

  libmicrohttpd::MHD_Daemon* daemon = nullptr;
  rest_service* service = nullptr;

  FILE* log_file = nullptr;
  std::mutex log_file_mutex;
  unsigned max_log_size = 0;

  unsigned min_generated = (1 << 10) + (1 << 8);
  unsigned max_connections = 0;
  unsigned max_post_size = 0;
  unsigned timeout = 0;
};

} // namespace microrestd
} // namespace ufal