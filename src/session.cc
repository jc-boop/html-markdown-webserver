#include <cstdlib>
#include <iostream>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <stdexcept>
#include "session.h"
#include "request_handler.h"
#include "echo_request_handler.h"
#include "error_request_handler.h"
#include "file_request_handler.h"
#include "request_parser.h"
#include "logging.h"

using boost::asio::ip::tcp;

tcp::socket& session::socket() {
    return socket_;
}

void session::start() {
  if (error_status > -1) {
    try {
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
          boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    catch (std::exception& e) {
      error_status = -1;
      log_exception(e);
    }
  }

}

void session::handle_read(const boost::system::error_code& error,
      size_t bytes_transferred) {
    if (!error) {
      try {
      // Call create request function
      http::request<http::string_body> req_;
      http::response<http::string_body> res_;
      std::string handler_name;
      if (create_request(data_, req_)) {
        // Create short-life handler (eventually replaced by Factory)
        auto request_handler_ = dispatcher_.get_handler(req_);
        handler_name = request_handler_->get_child_name();
        // Handle the request 
        http::status res_status_ = request_handler_->handle_request(req_, res_); 
      }
      else {
        res_.result(http::status::bad_request);
        handler_name = "badrequest";
        // Add this line if response hangs
        // res_.prepare_payload();
      }

      BOOST_LOG_TRIVIAL(trace) << "[ResponseMetrics] " << "response_code:" << res_.result_int() 
            << " request_path:" << req_.target() << " request_ip:" 
            << socket_.remote_endpoint().address().to_string() << " handler_name:" 
            << handler_name;

      http::write(socket_, res_);
      } catch (std::exception& e) {
        error_status = -1; 
        log_exception(e);
      } 
    }
    else {
      delete this;
    }
}

void session::handle_write(const boost::system::error_code& error) {
    if (!error) {
      try {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::handle_read, this,
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
      }
      catch (std::exception& e) {
        error_status = -1;
        log_exception(e);
      }
    }
    else {
      delete this;
    }
}

bool session::create_request(char* data_, http::request<http::string_body> &req) {
  // parse the request
  request_info info = parse_req_str(data_);
  
  if (info.parse_result) {
    auto verb = info.verb;
    std::string query = info.query;
    BOOST_LOG_TRIVIAL(info) << "Verb set to: " << verb << std::endl;
    BOOST_LOG_TRIVIAL(info) << "Query set to: " << query << std::endl;

    // Construct Beast request
    http::request<http::string_body> temp_req(verb, query, 11);
    temp_req.body() = info.body;
    temp_req.prepare_payload();

    // Log requested directory
    log_req_directory(info);

    req = temp_req;
    return true;
  }
  else {
    BOOST_LOG_TRIVIAL(warning) << "Invalid result from input string-to-request parsing" << std::endl;
    return false;
  }
  // return req;
}
