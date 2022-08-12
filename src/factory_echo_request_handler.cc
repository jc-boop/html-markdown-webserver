#include "factory_echo_request_handler.h"
#include "echo_request_handler.h"

std::unique_ptr<RequestHandler> EchoRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) {
    return std::unique_ptr<RequestHandler>(new EchoRequestHandler());
}