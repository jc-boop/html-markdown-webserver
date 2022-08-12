//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "server.h"
#include "session.h"
#include "config_parser.h"
#include "logging.h"
#include "signal.h"

using boost::asio::ip::tcp;

void sigint_handler(int signum) {
  log_interruption();
  exit(0);
}

void sigterm_handler(int signum) {
  log_termination();
  exit(0);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sigint_handler);
  signal(SIGTERM, sigterm_handler);

  try {
    init();

    if (argc != 2) {
      log_server_argument_usage();
      return 1;
    }

    boost::asio::io_service io_service;

    // Parse config file for port number
    NginxConfigParser parser;
    NginxConfig config;

    int port;
    if (!parser.Parse(argv[1], &config) || (port = parser.ExtractPort(argv[1])) <= -1) {
      log_missing_port_num();
      return -1;
    }

    // Start server on port number
    server s(io_service, static_cast<short>(port), std::string(argv[1]), 6);
    log_starting_server(port);
    s.run();
  }
  catch (std::exception& e) {
    log_exception(e);
  }

  return 0;
}
