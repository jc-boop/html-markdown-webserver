#ifndef MARKDOWN_REQUEST_HANDLER_FACTORY_H
#define MARKDOWN_REQUEST_HANDLER_FACTORY_H
#include "factory_request_handler.h"
#include "filesystem.h"
class MarkdownRequestHandlerFactory : public RequestHandlerFactory
{
public:
    MarkdownRequestHandlerFactory(const ArgsBuilder& args) 
      : RequestHandlerFactory(args) {}
    
    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name);
};
#endif // MARKDOWN_REQUEST_HANDLER_FACTORY_H
