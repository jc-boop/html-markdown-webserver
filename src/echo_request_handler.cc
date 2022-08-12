 
#include "echo_request_handler.h"

http::status EchoRequestHandler::serve(http::request<http::string_body> req, http::response<http::string_body>& res) {
  // Set response to 200 OK, HTTP 1.1, and echo the request
  res.result(http::status::ok);
  res.version(11);
  res.set(http::field::content_type, "text/plain");
  res.body() = req.body();
  res.prepare_payload();
  return http::status::ok;
}