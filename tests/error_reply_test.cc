#include "gtest/gtest.h"

EchoRequestHandler error_request_handler();

class ReplyTest : public ::testing::Test {
  protected:
    reply out_reply;
    std::vector<boost::asio::const_buffer> out_buffer;
};

TEST_F(ReplyTest, ErrorTest) {
  char input[18] = "gimli son of gloin";
  out_reply = echo_request_handler.handle_request(input, 18);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, http::status::not_found);
}
