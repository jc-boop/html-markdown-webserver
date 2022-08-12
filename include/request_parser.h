#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;
struct request_info {
  http::verb verb = http::verb::unknown;
  std::string query = "";
  std::string body = "";
  bool parse_result = true;
};

request_info parse_req_str(const char* req);

#endif // REQUEST_PARSER_H
