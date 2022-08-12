#ifndef ECHO_REQUEST_HANDLER_FACTORY_H
#define ECHO_REQUEST_HANDLER_FACTORY_H

#include "factory_request_handler.h"

class EchoRequestHandlerFactory : public RequestHandlerFactory
{
public:
    EchoRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}

    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};

#endif // ECHO_REQUEST_HANDLER_FACTORY_H