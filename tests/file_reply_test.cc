#include "gtest/gtest.h"
#include "file_request_handler.h"
#include "request_parser.h"
#include <boost/asio.hpp>


class FileReplyTest : public ::testing::Test {
  protected:
    reply out_reply;
    std::vector<boost::asio::const_buffer> out_buffer;
};

TEST_F(FileReplyTest, HandleFileHtml) {
  char request[] = "GET /static1/index.html HTTP/1.1\r\n\r\n";
  request_info info = parse_req_str(request);
  
  FileRequestHandler file_request_handler(info.base_dir,
                                          info.file_name, 1);
    
  out_reply = file_request_handler.handle_request(request, 0);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/html");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
}

TEST_F(FileReplyTest, HandleFileTxt) {
  char request[] = "GET /static1/index.txt HTTP/1.1\r\n\r\n";
  request_info info = parse_req_str(request);
  
  FileRequestHandler file_request_handler(info.base_dir,
                                          info.file_name, 1);
    
  out_reply = file_request_handler.handle_request(request, 0);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "text/txt");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
}

TEST_F(FileReplyTest, HandleFileZip) {
  char request[] = "GET /static1/index.zip HTTP/1.1\r\n\r\n";
  request_info info = parse_req_str(request);
  
  FileRequestHandler file_request_handler(info.base_dir,
                                          info.file_name, 1);
    
  out_reply = file_request_handler.handle_request(request, 0);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "application/zip");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
}

TEST_F(FileReplyTest, HandleFileJpg) {
  char request[] = "GET /static1/index.jpg HTTP/1.1\r\n\r\n";
  request_info info = parse_req_str(request);
  
  FileRequestHandler file_request_handler(info.base_dir,
                                          info.file_name, 1);
    
  out_reply = file_request_handler.handle_request(request, 0);
  out_buffer = out_reply.to_buffers();
  EXPECT_EQ(out_reply.status, reply::ok);
  EXPECT_EQ(out_reply.headers[0].name, "Content-Type");
  EXPECT_EQ(out_reply.headers[0].value, "image/jpeg");
  EXPECT_EQ(out_reply.headers[1].name, "Content-Length");
}
