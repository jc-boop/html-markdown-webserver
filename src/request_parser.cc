#include "request_parser.h"
#include <cstdlib>
#include <string>
#include "logging.h"

request_info parse_req_str(const char* req){
  // extract the actual request
  std::string data_str = std::string(req);
  std::string query_str = "";
  std::string verb_str = "";
  request_info req_info;
  size_t begin_pos;
  size_t end_pos;

  req_info.parse_result = true;

  // Check for existence of CRLF
  end_pos = data_str.find("\r\n\r\n");
  if (end_pos == std::string::npos) {
    req_info.parse_result = false;
  }

  // Set the body
  req_info.body = data_str.substr(end_pos+4);

  log_debug("crlf: " + std::to_string((req_info.parse_result) ? 1 : 0));

  // find the verb of the req
  end_pos = data_str.find_first_of(" ");
  if (end_pos != std::string::npos) {
    verb_str = data_str.substr(0, end_pos);
  }
  else {
    req_info.parse_result = false;
  }

  log_debug("verb: " + std::to_string((req_info.parse_result) ? 1 : 0));

  int verb_len = verb_str.length();
  for (int i = 0; i < verb_len; ++i) {
    if (isalpha(verb_str[i]))
      verb_str[i] = tolower(verb_str[i]);
  }
  log_debug("verb string: " + verb_str);

  // find the verb of req
  if (verb_str.find("get") != std::string::npos)
    req_info.verb = http::verb::get;
  else if (verb_str.find("post") != std::string::npos)
    req_info.verb = http::verb::post;
  //added by team tabs-not-spaces - begin
  else if (verb_str.find("delete") != std::string::npos)
    req_info.verb = http::verb::delete_;
  else if (verb_str.find("put") != std::string::npos)
    req_info.verb = http::verb::put;
  //added by team tabs-not-spaces - end
  else {
    BOOST_LOG_TRIVIAL(warning) << "Request parsing resulted in bad request! If you are sure method is valid, check that it's implemented in request_parser.cc!" << std::endl;
    req_info.parse_result = false;
  }
    
  log_debug("verb match: " + std::to_string((req_info.parse_result) ? 1 : 0));

  // find the query of the req with fomarting like "/folder/file" follow by a space
  begin_pos = data_str.find("/");
  if (begin_pos != std::string::npos) {
    end_pos = data_str.find(" ", begin_pos);

    log_debug("end pos: " + std::to_string(end_pos));
    if (end_pos != std::string::npos)
      query_str = data_str.substr(begin_pos, end_pos - begin_pos);
  }
  else {
    req_info.parse_result = false;
  }

  log_debug("path: " + std::to_string( (req_info.parse_result) ? 1 : 0));

  log_debug("data string: " + data_str);
  log_debug("query string: " + query_str);
  req_info.query = query_str;

  return req_info;
}
