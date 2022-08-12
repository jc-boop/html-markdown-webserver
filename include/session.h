#ifndef SESSION_H
#define SESSION_H

#include "request_handler.h"
#include "dispatcher.h"
#include <boost/beast/http.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <string>
namespace http = boost::beast::http;

using boost::asio::ip::tcp;

class session {
public:
  session(boost::asio::io_service& io_service, const std::string& config)
    : socket_(io_service), error_status(0), dispatcher_(config) {}

  tcp::socket& socket();
  void start();
  int get_error_status() {return error_status;}

private:
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  int error_status;
  Dispatcher dispatcher_;
  http::response<http::string_body> response_;
  bool create_request(char* data_, http::request<http::string_body> &req);
};

#endif
