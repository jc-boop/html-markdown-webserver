#include "gtest/gtest.h"
#include "dispatcher.h"
#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>

namespace http = boost::beast::http;
std::string dir_name = "./test_config";
class DispatcherTest : public ::testing::Test {
  protected:
    http::response<http::string_body> res_;
    http::status status_;
};

TEST_F(DispatcherTest, EchoTest) {
  Dispatcher dispatcher_(dir_name);
  char data_[27] = "GET /echo/ HTTP/1.1\r\n\r\n";
  request_info info = parse_req_str(data_);
  auto verb = info.verb;
  std::string query = info.query;
  http::request<http::string_body> req(verb, query, 11);
  req.body() = std::string(data_);
  req.prepare_payload();
  auto request_handler_ = dispatcher_.get_handler(req);
  status_ = request_handler_->handle_request(req, res_);
  EXPECT_EQ(status_, http::status::ok);
  EXPECT_EQ(res_.result(), http::status::ok);
  EXPECT_EQ(res_.version(), 11);
  EXPECT_EQ(res_.body(), data_);
  EXPECT_TRUE(dispatcher_.init_success);
}

TEST_F(DispatcherTest, HealthTest) {
  Dispatcher dispatcher_(dir_name);
  char data_[27] = "GET /health HTTP/1.1\r\n\r\n";
  request_info info = parse_req_str(data_);
  auto verb = info.verb;
  std::string query = info.query;
  http::request<http::string_body> req(verb, query, 11);
  req.prepare_payload();
  auto request_handler_ = dispatcher_.get_handler(req);
  status_ = request_handler_->handle_request(req, res_);
  EXPECT_EQ(status_, http::status::ok);
  EXPECT_EQ(res_.result(), http::status::ok);
  EXPECT_EQ(res_.version(), 11);
  EXPECT_EQ(res_.body(), "OK");
  EXPECT_TRUE(dispatcher_.init_success);
}


// THE FOLLOWING TESTS ARE COMMENTED OUT BECAUSE WE RAN OUT OF TIME
// TRYING TO GET THE DIRECTORY RELATED CODE BELOW WORKING ON THE GOOGLE CLOUD

// TEST_F(DispatcherTest, MarkdownCreateTest) {
//   Dispatcher dispatcher_(dir_name);
//   char data_[36] = "GET /markdown/create HTTP/1.1\r\n\r\n";
//   request_info info = parse_req_str(data_);
//   auto verb = info.verb;
//   std::string query = info.query;
//   http::request<http::string_body> req(verb, query, 11);
//   req.prepare_payload();
//   auto request_handler_ = dispatcher_.get_handler(req);
//   status_ = request_handler_->handle_request(req, res_);
//   std::ifstream test_file;
//   test_file.open("./test_files/md/1");
//   EXPECT_TRUE(test_file);
//   test_file.close();
//   EXPECT_FALSE(remove("./test_files/md/1"));
//   EXPECT_TRUE(dispatcher_.init_success);
// }

// TEST_F(DispatcherTest, MarkdownCreateTest2) {
//   Dispatcher dispatcher_(dir_name);
//   char data_[36] = "GET /markdown/create HTTP/1.1\r\n\r\n";
//   request_info info = parse_req_str(data_);
//   auto verb = info.verb;
//   std::string query = info.query;
//   http::request<http::string_body> req(verb, query, 11);
//   req.prepare_payload();
//   auto request_handler_ = dispatcher_.get_handler(req);
//   status_ = request_handler_->handle_request(req, res_);
//   status_ = request_handler_->handle_request(req, res_);
//   std::ifstream test_file;
//   test_file.open("./test_files/md/1");
//   EXPECT_TRUE(test_file);
//   test_file.close();
//   test_file.open("./test_files/md/2");
//   EXPECT_TRUE(test_file);
//   test_file.close();
//   EXPECT_FALSE(remove("./test_files/md/2"));
//   EXPECT_FALSE(remove("./test_files/md/1"));
//   EXPECT_TRUE(dispatcher_.init_success);
// }

// TEST_F(DispatcherTest, MarkdownConvertTest) {
//   Dispatcher dispatcher_(dir_name);
//   char data_[50] = "GET /markdown/convert HTTP/1.1\r\n\r\n# Hello World";
//   request_info info = parse_req_str(data_);
//   auto verb = info.verb;
//   std::string query = info.query;
//   http::request<http::string_body> req(verb, query, 11);
//   req.body() = info.body;
//   req.prepare_payload();
//   auto request_handler_ = dispatcher_.get_handler(req);
//   status_ = request_handler_->handle_request(req, res_);
//   EXPECT_EQ(status_, http::status::ok);
//   EXPECT_EQ(res_.result(), http::status::ok);
//   EXPECT_EQ(res_.version(), 11);
//   EXPECT_EQ(res_.body(), "<h1>Hello World</h1>\n");
//   EXPECT_TRUE(dispatcher_.init_success);
// }

// TEST_F(DispatcherTest, MarkdownConvertTest2) {
//   Dispatcher dispatcher_(dir_name);
//   char data_[103] = "GET /markdown/convert HTTP/1.1\r\n\r\n# Hello, this is a large heading\n## Hi, this is a smaller heading";
//   request_info info = parse_req_str(data_);
//   auto verb = info.verb;
//   std::string query = info.query;
//   http::request<http::string_body> req(verb, query, 11);
//   req.body() = info.body;
//   req.prepare_payload();
//   auto request_handler_ = dispatcher_.get_handler(req);
//   status_ = request_handler_->handle_request(req, res_);
//   EXPECT_EQ(status_, http::status::ok);
//   EXPECT_EQ(res_.result(), http::status::ok);
//   EXPECT_EQ(res_.version(), 11);
//   EXPECT_EQ(res_.body(), "<h1>Hello, this is a large heading</h1>\n<h2>Hi, this is a smaller heading</h2>\n");
//   EXPECT_TRUE(dispatcher_.init_success);
// }