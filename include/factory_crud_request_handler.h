#ifndef CRUD_REQUEST_HANDLER_FACTORY_H
#define CRUD_REQUEST_HANDLER_FACTORY_H

#include "factory_request_handler.h"
#include "filesystem.h"

class CRUDRequestHandlerFactory : public RequestHandlerFactory
{
public:
    CRUDRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}
    /*
    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url);
    */
    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};

#endif // CRUD_REQUEST_HANDLER_FACTORY_H