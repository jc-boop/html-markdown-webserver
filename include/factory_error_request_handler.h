#ifndef ERROR_REQUEST_HANDLER_FACTORY_H
#define ERROR_REQUEST_HANDLER_FACTORY_H

#include "factory_request_handler.h"

class ErrorRequestHandlerFactory : public RequestHandlerFactory
{
public:
    ErrorRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}

    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};

#endif // ERROR_REQUEST_HANDLER_FACTORY_H