#ifndef ERROR_REQUEST_HANDLER_H
#define ERROR_REQUEST_HANDLER_H

#include "request_handler.h"
#include <string>

class ErrorRequestHandler : public RequestHandler 
{
public:
  /// prohibit copying of request handlers
  ErrorRequestHandler(const ErrorRequestHandler&) = delete;
  ErrorRequestHandler& operator=(const ErrorRequestHandler&) = delete;

  /// Construct with a directory containing files to be served.
  ErrorRequestHandler()
    : RequestHandler() {}

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);
  std::string get_name() {
    return "errorhandler\n";
  }
};

#endif // ERROR_REQUEST_HANDLER_H
