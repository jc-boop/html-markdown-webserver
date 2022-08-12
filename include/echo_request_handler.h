
#ifndef ECHO_REQUEST_HANDLER_H
#define ECHO_REQUEST_HANDLER_H

#include "request_handler.h"
#include "logging.h"
#include <string>
#include <boost/beast/http.hpp>
namespace http = boost::beast::http;

class EchoRequestHandler : public RequestHandler 
{
public:
  /// prohibit copying of request handlers
  EchoRequestHandler(const EchoRequestHandler&) = delete;
  EchoRequestHandler& operator=(const EchoRequestHandler&) = delete;

  /// Construct with a directory containing files to be served.
  EchoRequestHandler()
    : RequestHandler() {}

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);
  std::string get_name() {
    return "echohandler\n";
  }
};

#endif // ECHO_REQUEST_HANDLER_H
