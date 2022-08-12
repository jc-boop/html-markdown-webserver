#include "factory_crud_request_handler.h"
#include "crud_request_handler.h"
/*
std::unique_ptr<RequestHandler> CRUDRequestHandlerFactory::create(
        const std::string& location, const std::string& url) {
    return std::unique_ptr<RequestHandler>(new CRUDRequestHandler(args_.root, location));
}
*/
std::unique_ptr<RequestHandler> CRUDRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) 
{
        FileSystem* fs =  new FileSystem();
        return std::unique_ptr<RequestHandler>(new CRUDRequestHandler(location, args_.root, fs));
}