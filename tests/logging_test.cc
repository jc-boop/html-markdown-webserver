#include "gtest/gtest.h"
#include "logging.h"
#include <boost/asio.hpp>
#include "session.h"
#include <iostream>
#include <fstream>

class LoggingTest : public :: testing::Test{
protected:
  std::ifstream logfile;
  char c;
};

TEST_F(LoggingTest, StartingServerTest){
  init();
  log_starting_server(8080);
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(info) Starting server on port: 8080\n");
}

TEST_F(LoggingTest, TerminationServerTest){
  init(); 
  log_termination();
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(info) Received SIGTERM. Shutting down\n");
}

TEST_F(LoggingTest, InterruptionTest){
  init(); 
  log_interruption();
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(info) Received SIGINT (Ctrl - C). Shutting down\n");
}

TEST_F(LoggingTest, IpAddressTest){
  init(); 
  log_ip_request("127.0.0.1", "GET /static1/index.zip HTTP/1.1");
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(trace) From IP: 127.0.0.1 Request: GET /static1/index.zip HTTP/1.1\n");
}

TEST_F(LoggingTest, DirectoryTest){
  init(); 
  request_info info;
  info.req_dir = "/echo/";
  log_req_directory(info);
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(info) HTTP Request Directory: /echo/\n");
}

TEST_F(LoggingTest, BadRequestTest){
  init(); 
  log_bad_request();
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(error) Bad request\n");
}

TEST_F(LoggingTest, AcceptFailTest){
  init(); 
  log_server_accept_fail();
  logfile.open("./log/SERVERLOG_0.log", std::ifstream::in);
  std::string body;
  while (logfile.get(c)) {
      body += c;
  }
  logfile.close();
  std::cerr << body << std::endl;
  int start_idx = body.rfind("]: ") + 3;
  std::string logged_message = body.substr(start_idx);
  EXPECT_EQ(logged_message, "(error) handle_accept argv[1]) failed\n");
}