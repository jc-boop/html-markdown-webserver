//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BASE_REQUEST_HANDLER_H
#define BASE_REQUEST_HANDLER_H

#include <string>
#include "logging.h"
#include <boost/beast/http.hpp>
namespace http = boost::beast::http;

// struct reply;
// struct request;

typedef http::request<http::string_body> request_t;
typedef http::response<http::string_body> response_t;

/// The common handler for all incoming requests.
class RequestHandler
{
public:
  /// prohibit copying of request handlers
  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  /// Construct with a directory containing files to be served.
  RequestHandler() {}

  /// Handle a request and produce a reply.
  http::status handle_request(http::request<http::string_body> req, http::response<http::string_body>& res);
  std::string get_child_name();

  

private:
  virtual http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res) = 0;
  virtual std::string get_name() {return "";} // todo
};

#endif // BASE_REQUEST_HANDLER_H
