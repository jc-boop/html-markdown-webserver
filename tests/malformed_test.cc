#include "gtest/gtest.h"
#include "request_parser.h"
#include <boost/asio.hpp>

class MalformedTest : public ::testing::Test {
};

TEST_F(MalformedTest, MissingCRLFTest) {
  char data_[26] = "GET /echo/ HTTP/1.1\\r\\n";
  request_info info = parse_req_str(data_);
  EXPECT_EQ(info.parse_result, false);
}

TEST_F(MalformedTest, InvalidVerbTest) {
  char data_[31] = "ET /echo/ HTTP/1.1\\r\\n\\r\\n";
  request_info info = parse_req_str(data_);
  EXPECT_EQ(info.parse_result, false);
}