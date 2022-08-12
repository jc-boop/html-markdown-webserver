#include "factory_file_request_handler.h"
#include "file_request_handler.h"

std::unique_ptr<RequestHandler> FileRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) {
    return std::unique_ptr<RequestHandler>(new FileRequestHandler(args_.root, file_name));
}