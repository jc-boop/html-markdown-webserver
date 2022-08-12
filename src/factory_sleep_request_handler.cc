#include "factory_sleep_request_handler.h"
#include "sleep_request_handler.h"

std::unique_ptr<RequestHandler> SleepRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) {
    return std::unique_ptr<RequestHandler>(new SleepRequestHandler(args_.root, file_name));
}