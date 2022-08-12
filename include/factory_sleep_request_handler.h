#ifndef SLEEP_REQUEST_HANDLER_FACTORY_H
#define SLEEP_REQUEST_HANDLER_FACTORY_H

#include "factory_request_handler.h"

class SleepRequestHandlerFactory : public RequestHandlerFactory
{
public:
    SleepRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}

    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};

#endif // SLEEP_REQUEST_HANDLER_FACTORY_H