 #include "sleep_request_handler.h"
 #include <iostream>
 #include <fstream>

http::status SleepRequestHandler::serve(http::request<http::string_body> req, 
                                       http::response<http::string_body>& res) {
  this->sleep_for_sec(10);  // sleep for 10 seconds
  
  res.result(http::status::ok);
  res.version(11);
  res.body() = "";
  res.prepare_payload();
  return http::status::ok;
}

void SleepRequestHandler::sleep_for_sec(int sec) {
  boost::this_thread::sleep_for(boost::chrono::seconds{sec});
}