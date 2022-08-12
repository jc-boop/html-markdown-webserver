 #include "file_request_handler.h"
 #include <iostream>
 #include <fstream>

http::status FileRequestHandler::serve(http::request<http::string_body> req, 
                                       http::response<http::string_body>& res) {
  std::string file_content = "";
  std::fstream req_file;
  req_file.open(base_dir_ + file_name_, std::ios::in);
  log_info("requested file: " + base_dir_ + file_name_);

  if(!req_file || file_name_ == "") {   // file not found
    res.result(http::status::not_found);
    res.version(11);
    res.set(http::field::content_type, "text/plain");
    res.body() = "." + base_dir_ + file_name_ + "\r\n\r\n";
    res.prepare_payload();
    return http::status::not_found;
  } 

  std::string line_str = "";

  while(getline(req_file, line_str)) {
    file_content += line_str + "\n";
  }

  req_file.close();

  // identify the type of the doc
  if (file_name_.find(".htm") != std::string::npos)
    res.set(http::field::content_type, "text/html");
  else if (file_name_.find(".txt") != std::string::npos)
    res.set(http::field::content_type, "text/txt");
  else if (file_name_.find(".zip") != std::string::npos)
    res.set(http::field::content_type, "application/zip");
  else if (file_name_.find(".pdf") != std::string::npos)
    res.set(http::field::content_type, "application/pdf");
  else if (file_name_.find(".jpg") != std::string::npos)
    res.set(http::field::content_type, "image/jpeg");
  else if (file_name_.find(".png") != std::string::npos)
    res.set(http::field::content_type, "image/png");
  else if (file_name_.find(".gif") != std::string::npos)
    res.set(http::field::content_type, "image/gif");
  
  res.result(http::status::ok);
  res.version(11);
  res.body() = file_content;
  res.prepare_payload();
  return http::status::ok;
}