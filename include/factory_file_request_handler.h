#ifndef STATIC_REQUEST_HANDLER_FACTORY_H
#define STATIC_REQUEST_HANDLER_FACTORY_H

#include "factory_request_handler.h"

class FileRequestHandlerFactory : public RequestHandlerFactory
{
public:
    FileRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}

    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};

#endif // STATIC_REQUEST_HANDLER_FACTORY_H