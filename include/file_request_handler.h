#ifndef FILE_REQUEST_HANDLER_H
#define FILE_REQUEST_HANDLER_H

#include "request_handler.h"
#include <string>

class FileRequestHandler : public RequestHandler 
{
public:
  /// prohibit copying of request handlers
  FileRequestHandler(const FileRequestHandler&) = delete;
  FileRequestHandler& operator=(const FileRequestHandler&) = delete;

  /// Construct with a directory containing files to be served.
  FileRequestHandler(const std::string& base_dir, const std::string& file)
    : RequestHandler(), base_dir_(base_dir), file_name_(file) {}

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);

  /// The base directory for navigating to the files to be served.
  std::string base_dir_;
  std::string file_name_;
  std::string get_name() {
    return "statichandler\n";
  }
};

#endif // FILE_REQUEST_HANDLER_H