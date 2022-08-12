#ifndef HEALTH_REQUEST_HANDLER_FACTORY_H
#define HEALTH_REQUEST_HANDLER_FACTORY_H

#include "factory_request_handler.h"

class HealthRequestHandlerFactory : public RequestHandlerFactory
{
public:
    HealthRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}

    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};

#endif // HEALTH_REQUEST_HANDLER_FACTORY_H