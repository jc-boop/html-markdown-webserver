#include "factory_error_request_handler.h"
#include "error_request_handler.h"

std::unique_ptr<RequestHandler> ErrorRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) {
    return std::unique_ptr<RequestHandler>(new ErrorRequestHandler());
}