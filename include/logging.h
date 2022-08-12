#ifndef LOG_H
#define LOG_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <exception>

#include "request_parser.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

const int TEN_MB = 10 * 1024 * 1024;

void init();
void log_server_argument_usage();
void log_missing_port_num();
void log_starting_server(int port);
void log_termination();
void log_interruption();
void log_exception(std::exception& e);
void log_token(std::string token_type, std::string token);
void log_bad_transition(std::string last_token_type, std::string token_type);
void log_bad_file(const char* file_name);
void log_ip_request(std::string ip_addr, std::string request);
void log_req_directory(request_info info);
void log_bad_request();
void log_server_accept_fail();
void log_dispatch_echo(std::string path);
void log_dispatch_file(std::string path);
void log_dispatch_error(std::string path);
void log_dispatch_crud(std::string path);
void log_dispatch_health(std::string path);
void log_debug(std::string msg);
void log_info(std::string msg);
#endif // LOG_H