#include "gtest/gtest.h"
#include "session.h"
#include "server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class ServerAndSessionTest : public ::testing::Test {
  protected:
    short port = 8080;
    // initialize an error code
    boost::system::error_code ec = boost::system::errc::make_error_code(
                                  boost::system::errc::not_supported) ;
};

TEST_F(ServerAndSessionTest, TestServerStartAccpet) {
    boost::asio::io_service io_service;
    server test_server(io_service, port);
    EXPECT_EQ(0, test_server.get_error_status());
}

TEST_F(ServerAndSessionTest, TestSessionStart) {
    boost::asio::io_service io_service;
    session test_session(io_service);
    test_session.start();
    EXPECT_EQ(0, test_session.get_error_status());
}


