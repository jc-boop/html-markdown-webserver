#include "logging.h"
#include "request_parser.h"

void init() {
  logging::add_file_log(
    keywords::file_name = "log/SERVERLOG_%N.log",
    keywords::rotation_size = TEN_MB,
    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
    keywords::format = "[%TimeStamp%]:Thread[%ThreadID%]: (%Severity%) %Message%",
    keywords::auto_flush = true
  );
  
  logging::add_console_log(
    std::cout, 
    keywords::format = "[%TimeStamp%]:Thread[%ThreadID%]: (%Severity%) %Message%",
    keywords::auto_flush = true
  );

  logging::add_common_attributes();
}

void log_server_argument_usage() {
  BOOST_LOG_TRIVIAL(error) << "Usage: async_tcp_echo_server <port>\n";
}

void log_missing_port_num() {
  BOOST_LOG_TRIVIAL(error) << "Cannot find port number in config file\n";
}

void log_starting_server(int port) {
  BOOST_LOG_TRIVIAL(info) << "Starting server on port: " << port << std::endl;
}

void log_termination() {
  BOOST_LOG_TRIVIAL(info) << "Received SIGTERM. Shutting down\n";
}

void log_interruption() {
  BOOST_LOG_TRIVIAL(info) << "Received SIGINT (Ctrl - C). Shutting down\n";
}

void log_exception(std::exception& e) {
  BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what() << "\n";
}

void log_token(std::string token_type, std::string token) {
  BOOST_LOG_TRIVIAL(trace) << token_type << ": " << token.c_str() << std::endl;
}

void log_bad_transition(std::string last_token_type, std::string token_type) {
  BOOST_LOG_TRIVIAL(error) << "Bad transition from " << last_token_type << 
    " to " << token_type << std::endl;
}

void log_bad_file(const char* file_name) {
  BOOST_LOG_TRIVIAL(error) << "Failed to open config file: " << file_name << std::endl;
}

void log_ip_request(std::string ip_addr, std::string request) {
  BOOST_LOG_TRIVIAL(trace) << "From IP: " << ip_addr << " Request: " << request;
}

void log_req_directory(request_info info) {
  BOOST_LOG_TRIVIAL(info) << "HTTP Request Directory: " << info.query << std::endl;
}

void log_bad_request() {
  BOOST_LOG_TRIVIAL(error) << "Bad request\n";
}

void log_server_accept_fail() {
  BOOST_LOG_TRIVIAL(error) << "handle_accept argv[1]) failed\n";
}

void log_dispatch_echo(std::string path) {
  BOOST_LOG_TRIVIAL(info) << "Dispatched Echo Handler Factory on path " << path << std::endl;
}

void log_dispatch_file(std::string path) {
  BOOST_LOG_TRIVIAL(info) << "Dispatched File Handler Factory on path " << path << std::endl;
}

void log_dispatch_error(std::string path) {
  BOOST_LOG_TRIVIAL(info) << "Dispatched Error Handler Factory on path " << path << std::endl;
}

void log_dispatch_crud(std::string path) {
  BOOST_LOG_TRIVIAL(info) << "Dispatched CRUD Handler Factory on path " << path << std::endl;
}

void log_dispatch_health(std::string path) {
  BOOST_LOG_TRIVIAL(info) << "Dispatched Health Handler Factory on path " << path << std::endl;
}

void log_debug(std::string msg) {
  BOOST_LOG_TRIVIAL(debug) << msg << std::endl;
}

void log_info(std::string msg) {
  BOOST_LOG_TRIVIAL(info) << msg << std::endl;
}
