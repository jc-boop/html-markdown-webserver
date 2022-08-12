#include "gtest/gtest.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "server.h"
#include "session.h"
#include "config_parser.h"



class NginxConfigParserTest : public ::testing::Test {
  protected:

    NginxConfigParser parser;
    NginxConfig out_config;   
    boost::asio::io_service io_service; 

};

TEST_F(NginxConfigParserTest, SmallCheck) {
  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, CheckSingleQuoteInsideDoubleQuote) {
  bool success = parser.Parse("config1", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, CheckExtraSemicolon) {
  bool success = parser.Parse("config2", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CheckExtraSemicolonBracket) {
  bool success = parser.Parse("config3", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CheckExtraDoubleQuote) {
  bool success = parser.Parse("config4", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CheckBackslashEscaped) {
  bool success = parser.Parse("config5", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, CheckEmptyBrace) {
  bool success = parser.Parse("config6", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NestedBracketConfig) {
  bool success = parser.Parse("nested_bracket_config", &out_config);
  EXPECT_TRUE(success);
}
TEST_F(NginxConfigParserTest, EscapedQuoteConfig) {
  bool success = parser.Parse("escaped_quote_config", &out_config);
  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, BadConfig) {
  bool success = parser.Parse("bad_config", &out_config);
  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CheckPortNum) {
  //parser.Parse("config2", &out_config);
  int port_num = parser.ExtractPort("config2");
  EXPECT_TRUE(port_num ==  80);
}

TEST_F(NginxConfigParserTest, CheckPortNum2) {
  //parser.Parse("config7", &out_config);
  int port_num = parser.ExtractPort("config7");
  EXPECT_TRUE(port_num ==  8080);
}

TEST_F(NginxConfigParserTest, CheckPortNum3) {
  //parser.Parse("config8", &out_config);
  int port_num = parser.ExtractPort("config8");
  EXPECT_TRUE(port_num ==  62000);
}

TEST_F(NginxConfigParserTest, CheckPortNum4) {
  int port_num = parser.ExtractPort("config9");
  EXPECT_TRUE(port_num ==  8080);
}

TEST_F(NginxConfigParserTest, CheckPortNum5) {
  int port_num = parser.ExtractPort("config10");
  EXPECT_TRUE(port_num ==  8080);
}

TEST_F(NginxConfigParserTest, CheckPortNum6) {
  int port_num = parser.ExtractPort("config11");
  EXPECT_TRUE(port_num ==  8080);
}

TEST_F(NginxConfigParserTest, ExtractRoot) {
  location_info info = parser.ExtractLocation("location_config");
  EXPECT_EQ(info.handler_to_root["StaticHandler"], "../tests/server_files");
}

TEST_F(NginxConfigParserTest, ExtractRoot2) {
  location_info info = parser.ExtractLocation("location_config2");
  EXPECT_EQ(info.handler_to_root["StaticHandler"], "/home/ubuntu/sites/foo/");
}