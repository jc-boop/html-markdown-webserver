#ifndef HEALTH_REQUEST_HANDLER_H
#define HEALTH_REQUEST_HANDLER_H

#include "request_handler.h"
#include <string>

class HealthRequestHandler : public RequestHandler 
{
public:
  /// prohibit copying of request handlers
  HealthRequestHandler(const HealthRequestHandler&) = delete;
  HealthRequestHandler& operator=(const HealthRequestHandler&) = delete;

  /// Construct with a directory containing files to be served.
  HealthRequestHandler()
    : RequestHandler() {}

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);
  std::string get_name() {
    return "healthhandler\n";
  }
};

#endif
