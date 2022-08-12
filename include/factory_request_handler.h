#ifndef BASE_REQUEST_HANDLER_FACTORY_H
#define BASE_REQUEST_HANDLER_FACTORY_H

#include "request_handler.h"
#include <string>

namespace http = boost::beast::http;

struct ArgsBuilder {
    std::string root;
};

class RequestHandlerFactory 
{
public:
    RequestHandlerFactory(const ArgsBuilder& args) 
      : args_(args) {}

    virtual std::unique_ptr<RequestHandler> create(const std::string& location, 
        const std::string& url, const std::string& file_name) = 0;

protected:
    // each factory has an associated location
    ArgsBuilder args_;
};

#endif // BASE_REQUEST_HANDLER_FACTORY_H