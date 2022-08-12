#ifndef MARKDOWN_REQUEST_HANDLER_H
#define MARKDOWN_REQUEST_HANDLER_H

#include "request_handler.h"
#include "filesystem_interface.h"
#include <string>

class MarkdownRequestHandler : public RequestHandler 
{
public:
  MarkdownRequestHandler(const MarkdownRequestHandler&) = delete;
  MarkdownRequestHandler& operator=(const MarkdownRequestHandler&) = delete;

  // MarkdownRequestHandler(const std::string& location, const std::string& data_path)
  //   : RequestHandler(), location_(location), data_path_(data_path) {}

  MarkdownRequestHandler(const std::string& location, const std::string& data_path, FileSystemInterface* filesystem)
    : RequestHandler(), location_(location), data_path_(data_path), filesystem_(filesystem) {}

  // // Destructor
  ~MarkdownRequestHandler() {
    delete filesystem_;
  }

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status create(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status convert(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status view(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status list(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status save(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status edit(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status md_delete(http::request<http::string_body> req, http::response<http::string_body>& res);
  std::string map_request_path_to_file_path(std::string request_uri);
  std::string extract_text(std::string str);
  std::string extract_file_name(std::string str);
  std::string construct_file_path(std::string filename);
  FileSystemInterface* filesystem_;
  std::string data_path_;
  std::string location_;
  std::string convert_(std::string request_markdown);

  std::string get_name() {
    return "markdownhandler\n";
  }
};

#endif // MARKDOWN_REQUEST_HANDLER_H
