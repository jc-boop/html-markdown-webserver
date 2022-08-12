#include "factory_markdown_request_handler.h"
#include "markdown_request_handler.h"

std::unique_ptr<RequestHandler> MarkdownRequestHandlerFactory::create(
        const std::string& location, const std::string& url, const std::string& file_name) {

        FileSystem* fs =  new FileSystem();
        return std::unique_ptr<RequestHandler>(new MarkdownRequestHandler(location, args_.root, fs));
}