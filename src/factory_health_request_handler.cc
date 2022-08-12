#include "factory_health_request_handler.h"
#include "health_request_handler.h"

std::unique_ptr<RequestHandler> HealthRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) {
    return std::unique_ptr<RequestHandler>(new HealthRequestHandler());
}