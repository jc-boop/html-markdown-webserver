#include "gtest/gtest.h"
#include "crud_request_handler.h"
#include "request_parser.h"
#include "mock_filesystem.h"
#include <boost/asio.hpp>

class CRUDReplyTest : public ::testing::Test {
  protected:
    request_t request;
    response_t response;
};

TEST_F(CRUDReplyTest, CreateTestSuccess) {
  MockFileSystem* mock = new MockFileSystem;

  auto verb = http::verb::post;
  std::string path = "/api/food";
  std::string data = "{ \"name\" : \"chipotle\" }";

  http::request<http::string_body> req_post(verb, path, 11);
  req_post.set(http::field::content_type, "application/json");
  req_post.body() = data;

  EXPECT_CALL(*mock, writeFile("/server_files3/food/1", data))
      .Times(1)
      .WillOnce(testing::Return(true));

  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::created);

}

TEST_F(CRUDReplyTest, RetrieveNotFound) {
  MockFileSystem* mock = new MockFileSystem;

  auto verb = http::verb::get;
  std::string path = "/api/shoes/10";
  http::request<http::string_body> req_post(verb, path, 11);

  EXPECT_CALL(*mock, readFile("/server_files3/shoes/10", testing::_))
      .Times(1)
      .WillOnce(testing::Return(false));

    
  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::not_found);
}

TEST_F(CRUDReplyTest, RetrieveSuccess) {
  MockFileSystem* mock = new MockFileSystem;

  auto verb = http::verb::get;
  std::string path = "/api/shoes/1";
  http::request<http::string_body> req_post(verb, path, 11);

  // std::string data = "{ \"price\" : 50 }";
  EXPECT_CALL(*mock, readFile("/server_files3/shoes/1", testing::_))
      .Times(1)
      .WillOnce(testing::Return(true));
  // EXPECT_CALL(mock, readFile("../server_files3/shoes/1", testing::_))
  //     .Times(1)
  //  .WillOnce(testing::DoAll(testing::SetArgReferee<1>(data), testing::Return(true)));

  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::ok);
}

// TEST_F(CRUDReplyTest, DeleteNotFound) {
//   auto verb = http::verb::delete_;
//   std::string path = "/api/shoes/10";
//   http::request<http::string_body> req_post(verb, path, 11);

//   EXPECT_CALL(mock, deleteFile("/server_files3/shoes/10"))
//       .Times(1)
//       .WillOnce(testing::Return(false));
    
//   CRUDRequestHandler crud_request_handler("/api", "/server_files3", &mock);
//   crud_request_handler.handle_request(req_post, response);
//   EXPECT_EQ(response.result(), http::status::not_found);
// }

TEST_F(CRUDReplyTest, DeleteSuccess) {
  MockFileSystem* mock = new MockFileSystem;

  auto verb = http::verb::delete_;
  std::string path = "/api/shoes/1";
  http::request<http::string_body> req_post(verb, path, 11);

  EXPECT_CALL(*mock, deleteFile("/server_files3/shoes/1"))
      .Times(1)
      .WillOnce(testing::Return(true));

  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::ok);
}

TEST_F(CRUDReplyTest, ListEmptyDirectory) {
  MockFileSystem* mock = new MockFileSystem;
  
  auto verb = http::verb::get;
  std::string path = "/api/shoes";
  http::request<http::string_body> req_post(verb, path, 11);

  EXPECT_CALL(*mock, listFiles("/server_files3/shoes", testing::_))
      .Times(1)
      .WillOnce(testing::Return(false));
    
  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::ok);
  EXPECT_EQ(response.body(), "[]"); 
}

TEST_F(CRUDReplyTest, ListSuccess) {
  MockFileSystem* mock = new MockFileSystem;

  auto verb = http::verb::get;
  std::string path = "/api/shoes";
  http::request<http::string_body> req_post(verb, path, 11);

  EXPECT_CALL(*mock, listFiles("/server_files3/shoes", testing::_))
      .Times(1)
      .WillOnce(testing::Return(true));

  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::ok);
}


TEST_F(CRUDReplyTest, UpdateSuccess) {
  MockFileSystem* mock = new MockFileSystem;

  auto verb = http::verb::put;
  std::string path = "/api/shoes/200";
  std::string data = "{ \"name\" : \"heels\" }";
  http::request<http::string_body> req_post(verb, path, 11);

  req_post.set(http::field::content_type, "application/json");
  req_post.body() = data;

  EXPECT_CALL(*mock, writeFile("/server_files3/shoes/200", data))
      .Times(1)
      .WillOnce(testing::Return(true));

  CRUDRequestHandler crud_request_handler("/api", "/server_files3", mock);
  crud_request_handler.handle_request(req_post, response);
  EXPECT_EQ(response.result(), http::status::ok);
}
