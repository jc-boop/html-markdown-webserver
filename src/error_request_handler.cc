 #include "error_request_handler.h"

http::status ErrorRequestHandler::serve(http::request<http::string_body> req, http::response<http::string_body>& res) {
  // Set reply to 404, HTTP 1.1, and echo the request
  res.result(http::status::not_found);
  res.version(11);
  res.set(http::field::content_type, "text/plain");
  res.body() = req.body();
  res.prepare_payload();
  return res.result();
}