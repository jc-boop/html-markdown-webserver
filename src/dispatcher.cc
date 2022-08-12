#include "dispatcher.h"

#include "echo_request_handler.h"
#include "file_request_handler.h"
#include "error_request_handler.h"
#include "crud_request_handler.h"
#include "sleep_request_handler.h"
#include "factory_echo_request_handler.h"
#include "factory_file_request_handler.h"
#include "factory_error_request_handler.h"
#include "factory_crud_request_handler.h"
#include "factory_health_request_handler.h"
#include "factory_sleep_request_handler.h"
#include "factory_markdown_request_handler.h"
#include "logging.h"
#include <map>
#include <iterator>

std::shared_ptr<RequestHandler> Dispatcher::get_handler(http::request<http::string_body> req) {
  
  // if (req.)
  // Fetch the Beast request URI
  std::string request_uri = {req.target().begin(), req.target().end()};

  // Split uri into two parts
  // 1. path with longest matching prefix
  // 2. file name (remaining part of the uri)
  std::string matched_path = request_uri;
  std::string file_name = split_uri(matched_path);

  std::shared_ptr<RequestHandlerFactory> dispatched_handler_factory = nullptr;
  if (routes.find(matched_path) != routes.end()) {
    dispatched_handler_factory = routes[matched_path];
  }
  // Error Handler if no path found
  else {
    dispatched_handler_factory = routes["/"];
  }

  // log_debug("longest matched path: " + matched_path + "; file path: " + file_name);

  return dispatched_handler_factory->create(matched_path, request_uri, file_name);
}

Dispatcher::Dispatcher(std::string config_file_path) {
  init_success = initialize_handlers(config_file_path.c_str());
}

bool Dispatcher::initialize_handlers(const char* config_file_path) {
  // parser for the default config file path
  NginxConfigParser parser;
  handler_info = parser.ExtractLocation(config_file_path);

  // iterate the maps in the info struct
  for (auto itr = handler_info.path_to_handler.begin(); 
       itr != handler_info.path_to_handler.end(); ++itr) { 
    log_debug("path name: " + itr->first);
    log_debug("handler name: " + itr->second);
    dispatch(itr->first, itr->second); 
  }

  return true;
}

bool Dispatcher::dispatch(std::string path, std::string handler) {
  std::cout << "Dispatcher Path: " << path << std::endl;
  trim_slashes(path);

  if (routes.find(path) != routes.end()) {
    return false; 
  }

  ArgsBuilder args;
  // Instantiate handlers based on type requested
  if (handler == "EchoHandler") {
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new EchoRequestHandlerFactory(args));
    log_dispatch_echo(path);
    return true;
  }
  else if (handler == "StaticHandler") {
    // find the root for the particular handler
    args.root = handler_info.path_to_root[path];
    log_debug(args.root);
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new FileRequestHandlerFactory(args));
    log_dispatch_file(path);
    return false;
  }
  else if (handler == "ErrorHandler") {
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new ErrorRequestHandlerFactory(args));
    log_dispatch_error(path);
    return true;
  }
  else if (handler == "CRUDHandler") {
    args.root = handler_info.path_to_root[path];
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new CRUDRequestHandlerFactory(args));
    log_dispatch_crud(path);
    return true;
  }
  else if (handler == "HealthHandler") {
    args.root = handler_info.path_to_root[path];
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new HealthRequestHandlerFactory(args));
    log_dispatch_health(path);
    return true;
  }
  else if (handler == "SleepHandler") {
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new SleepRequestHandlerFactory(args));
    log_dispatch_error(path);
    return true;
  }
  else if (handler == "MarkdownHandler") {
    args.root = handler_info.path_to_root[path];
    routes[path] = std::shared_ptr<RequestHandlerFactory>(new MarkdownRequestHandlerFactory(args));
    log_dispatch_error(path);
    return true;
  }
  else
    return false;
}

void Dispatcher::trim_slashes(std::string &uri) {
  if (uri != "/") {
    while (uri.length() > 1 && uri.back() == '/') {
      uri.pop_back();
    }
  }
}

std::string Dispatcher::split_uri(std::string& uri) {
  std::string uri_substring;
  std::string matched_path = "";

  // Perform longest matching prefix with directories from the map
  for (auto item = routes.begin(); item != routes.end(); ++item) {
    size_t item_len = item->first.length();
    uri_substring = uri.substr(0, item_len);
    if ((uri_substring == item->first) && (item_len > matched_path.length())) {
      matched_path = uri_substring;
    }
  }

  std::string file_name = uri.substr(matched_path.length());
  uri = matched_path;
  
  return file_name;
}
