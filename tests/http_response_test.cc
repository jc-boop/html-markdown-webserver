#include "gtest/gtest.h"
#include "http_response.h"
#include "session.h"
#include <boost/asio.hpp>

boost::asio::io_service io_service;

class ResponseTest : public ::testing::Test {
  protected:
    http_response out_response;
    std::vector<boost::asio::const_buffer> out_buffer;
};

TEST_F(ResponseTest, HelloWorldTest) {
  session test_session(io_service);
  char input[12] = "Hello World";
  out_response = test_session.echo_response(input, 12);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 12);
  out_buffer = out_response.convert_into_buffer();
  EXPECT_TRUE(success);
}

TEST_F(ResponseTest, NumbersTest) {
  session test_session(io_service);
  char input[18] = "1 2 3 4 5 6 7 8 9";
  out_response = test_session.echo_response(input, 18);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 18);
  out_buffer = out_response.convert_into_buffer();
  EXPECT_TRUE(success);
}

TEST_F(ResponseTest, SymbolsTest) {
  session test_session(io_service);
  char input[22] = "# ^ & @@ *& ()@ #_%\\";
  out_response = test_session.echo_response(input, 22);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 22);
  out_buffer = out_response.convert_into_buffer();
  EXPECT_TRUE(success);
}

TEST_F(ResponseTest, LinebreaksTest) {
  session test_session(io_service);
  char input[33] = "\n \n \n \n \n \r \r \r \n \n \r";
  out_response = test_session.echo_response(input, 33);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 33);
  out_buffer = out_response.convert_into_buffer();
  EXPECT_TRUE(success);
}

TEST_F(ResponseTest, NotstringsTest) {
  session test_session(io_service);
  int input = 1337;
  char* cp = reinterpret_cast<char*>(&input);
  out_response = test_session.echo_response(cp, 4);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 4);
  out_buffer = out_response.convert_into_buffer();
  
  EXPECT_FALSE(success);
}

TEST_F(ResponseTest, CutoffinputsTest) {
  session test_session(io_service);
  char input[33] = "\n \n \n \n \n \r \r \r \n \n \r";
  out_response = test_session.echo_response(input, 2);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 33);
  out_buffer = out_response.convert_into_buffer();
  EXPECT_FALSE(success);
}

TEST_F(ResponseTest, NoinputsTest) {
  session test_session(io_service);
  char input[1] = "";
  out_response = test_session.echo_response(input, 1);
  bool success = out_response.header[0].header_name == "HTTP/1.1" &&  out_response.header[0].header_val == "200 OK" &&  out_response.header[1].header_name == "Content-Type:" &&  out_response.header[1].header_val == "text/plain" && out_response.body == std::string(input, 1);
  out_buffer = out_response.convert_into_buffer();
  EXPECT_TRUE(success);
}