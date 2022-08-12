#ifndef SERVER_H
#define SERVER_H

#include "session.h"
#include <string>

using boost::asio::ip::tcp;

class server {
public:
  server(boost::asio::io_service& io_service, short port,
         const std::string & config, const int& thread_size);
  int get_error_status() {return error_status;}
  void run();

private:
  void start_accept();
  void handle_accept(session* new_session,
                     const boost::system::error_code& error);
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  int error_status;
  std::string config_file;
  int thread_pool_size_;
};

#endif
