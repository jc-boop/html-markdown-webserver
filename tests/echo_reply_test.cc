#include "gtest/gtest.h"
#include "echo_request_handler.h"
#include <boost/asio.hpp>

EchoRequestHandler echo_request_handler("/echo/", "", 0);

class ReplyTest : public ::testing::Test {
  protected:
    reply out_reply;
    std::vector<boost::asio::const_buffer> out_buffer;
};

TEST_F(ReplyTest, HelloWorldTest) {
  char input[12] = "Hello World";
  out_reply = echo_request_handler.handle_request(input, 12);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_EQ(out_reply.content, std::string(input, 12));
}

TEST_F(ReplyTest, NumbersTest) {
  char input[18] = "1 2 3 4 5 6 7 8 9";
  out_reply = echo_request_handler.handle_request(input, 18);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_EQ(out_reply.content, std::string(input, 18));
}

TEST_F(ReplyTest, SymbolsTest) {
  char input[22] = "# ^ & @@ *& ()@ #_%\\";
  out_reply = echo_request_handler.handle_request(input, 22);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_EQ(out_reply.content, std::string(input, 22));
}

TEST_F(ReplyTest, LinebreaksTest) {
  char input[33] = "\n \n \n \n \n \r \r \r \n \n \r";
  out_reply = echo_request_handler.handle_request(input, 33);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_EQ(out_reply.content, std::string(input, 33));
}

TEST_F(ReplyTest, NotstringsTest) {
  int input = 1337;
  char* cp = reinterpret_cast<char*>(&input);
  out_reply = echo_request_handler.handle_request(cp, 4);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_NE(out_reply.content, std::string(input, 4));
}

TEST_F(ReplyTest, CutoffinputsTest) {
  char input[33] = "\n \n \n \n \n \r \r \r \n \n \r";
  out_reply = echo_request_handler.handle_request(input, 2);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_NE(out_reply.content, std::string(input, 33));
}

TEST_F(ReplyTest, NoinputsTest) {
  char input[1] = "";
  out_reply = echo_request_handler.handle_request(input, 1);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/plain");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
  EXPECT_EQ(out_reply.headers[1].value, std::to_string(out_reply.content.size()));
  EXPECT_EQ(out_reply.content, std::string(input, 1));
}
