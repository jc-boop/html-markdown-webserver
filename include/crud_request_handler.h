#ifndef CRUD_REQUEST_HANDLER_H
#define CRUD_REQUEST_HANDLER_H

#include "request_handler.h"
#include "filesystem_interface.h"
#include <string>

class CRUDRequestHandler : public RequestHandler 
{
public:
  CRUDRequestHandler(const CRUDRequestHandler&) = delete;
  CRUDRequestHandler& operator=(const CRUDRequestHandler&) = delete;

  CRUDRequestHandler(const std::string& location, const std::string& data_path, FileSystemInterface* filesystem)
    : RequestHandler(), location_(location), data_path_(data_path), filesystem_(filesystem) {}

  // Destructor
  ~CRUDRequestHandler() {
    delete filesystem_;
  }

private:
  http::status serve(http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status create(std::string entity, http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status retrieve(std::string entity_id, http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status update(std::string entity_id, http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status crud_delete(std::string entity_id, http::request<http::string_body> req, http::response<http::string_body>& res);
  http::status list(std::string entity, http::request<http::string_body> req, http::response<http::string_body>& res);
  std::string map_request_path_to_file_path(std::string request_uri); 
  std::stringbuf get_JSON_id(int id);

  std::string data_path_;
  std::string location_;
  FileSystemInterface* filesystem_;

  std::string get_name() {
    return "crudhandler\n";
  }
};

#endif // CRUD_REQUEST_HANDLER_H