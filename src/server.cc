#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "server.h"
#include "logging.h"

using boost::asio::ip::tcp;

server::server(boost::asio::io_service& io_service, short port,
               const std::string & config, const int& thread_size)
    : io_service_(io_service), error_status(0), config_file(config),
      thread_pool_size_(thread_size),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void server::start_accept() {
    session* new_session = new session(io_service_, config_file);

    try {
      acceptor_.async_accept(new_session->socket(),
                boost::bind(&server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }
    catch (std::exception& e) {
      error_status = -1;
      log_exception(e);
    }
}

void server::handle_accept(session* new_session,
                           const boost::system::error_code& error) {
    if (!error) {
      try {
        new_session->start(); // handle asyn read
      }
      catch (std::exception& e) {
        error_status = -1;
        log_exception(e);
      }
    }
    else {
      error_status = -1;
      log_server_accept_fail();
      delete new_session;
      new_session = nullptr;
    }

    start_accept();
}

void server::run() {

  log_info("multi-threading in run");
  
  // Create a pool of threads to run all of the io_services.
  std::vector<boost::shared_ptr<boost::thread> > threads;
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    boost::shared_ptr<boost::thread> thread(new boost::thread(
          boost::bind(&boost::asio::io_service::run, &io_service_)));
    threads.push_back(thread);
  }

  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < threads.size(); ++i)
    threads[i]->join();
  
  
}