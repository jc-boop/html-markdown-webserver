#include "request_handler.h"
#include <string>
#include <fstream>
#include <iostream>

typedef bool status;

http::status RequestHandler::handle_request(http::request<http::string_body> req, 
                                            http::response<http::string_body>& res) {
  http::status return_status = serve(req, res);
  return return_status;
}

std::string RequestHandler::get_child_name() {
  return get_name();
}