#ifndef SLEEP_REQUEST_HANDLER_H
#define SLEEP_REQUEST_HANDLER_H

#include "request_handler.h"
#include <string>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

class SleepRequestHandler : public RequestHandler 
{
public:
  /// prohibit copying of request handlers
  SleepRequestHandler(const SleepRequestHandler&) = delete;
  SleepRequestHandler& operator=(const SleepRequestHandler&) = delete;

  /// Construct with a directory containing files to be served.
  SleepRequestHandler(const std::string& base_dir, const std::string& file)
    : RequestHandler(), base_dir_(base_dir), file_name_(file) {}
  
  

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);
  void sleep_for_sec(int sec);
  std::string get_name() {
    return "sleephandler\n";
  }

  /// The base directory for navigating to the files to be served.
  std::string base_dir_;
  std::string file_name_;
};

#endif // FILE_REQUEST_HANDLER_H