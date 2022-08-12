#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <iostream> 
#include <string_view>
#include <map>
#include <boost/beast/http.hpp>
#include "config_parser.h"
#include "request_handler.h"
#include "factory_request_handler.h"

namespace http = boost::beast::http;

class Dispatcher {
public:
    bool initialize_handlers(const char* config_file_path); 
    bool init_success = false; 
    Dispatcher(std::string config_file_path);
    virtual std::shared_ptr<RequestHandler> get_handler(http::request<http::string_body> req);

private:
    std::map<std::string, std::shared_ptr<RequestHandlerFactory>> routes;
    bool dispatch(std::string path, std::string handler);
    void trim_slashes(std::string& uri);
    std::string split_uri(std::string& uri);
    location_info handler_info;
    
};

#endif