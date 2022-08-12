 #include "health_request_handler.h"

http::status HealthRequestHandler::serve(http::request<http::string_body> req, http::response<http::string_body>& res) {
  (http::status::ok);
  res.version(11);
  res.set(http::field::content_type, "text/plain");
  res.body() = "OK";
  res.prepare_payload();
  return res.result();
}