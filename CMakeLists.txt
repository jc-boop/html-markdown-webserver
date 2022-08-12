cmake_minimum_required(VERSION 3.10.0)
project(ffs)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system filesystem REQUIRED log log_setup regex)
find_package(Python COMPONENTS Interpreter Development)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)
include_directories(cpp_markdown)

# TODO(!): Update name and srcs
add_library(server_lib src/server.cc)
add_library(session_lib src/session.cc)
add_library(config_parser_lib src/config_parser.cc)
add_library(request_parser_lib src/request_parser.cc)
add_library(logging_lib src/logging.cc)
add_library(dispatcher_lib src/dispatcher.cc)
add_library(filesystem_lib src/filesystem.cc)
add_library(request_handler_lib 
                src/request_handler.cc 
                src/echo_request_handler.cc 
                src/error_request_handler.cc 
                src/file_request_handler.cc
                src/crud_request_handler.cc
                src/health_request_handler.cc
                src/sleep_request_handler.cc
                src/markdown_request_handler.cc)
add_library(factory_request_handler_lib 
                src/factory_echo_request_handler.cc
                src/factory_error_request_handler.cc
                src/factory_file_request_handler.cc
                src/factory_crud_request_handler.cc
                src/factory_health_request_handler.cc
                src/factory_sleep_request_handler.cc
                src/factory_markdown_request_handler.cc)
add_library(cppmarkdown
	cpp_markdown/markdown.h
	cpp_markdown/markdown.cpp
	cpp_markdown/markdown-tokens.h
	cpp_markdown/markdown-tokens.cpp
)
# TODO(!): Update executable name, srcs, and deps
add_executable(server src/server_main.cc)

target_link_libraries(cppmarkdown Boost::regex)
target_link_libraries(factory_request_handler_lib request_handler_lib filesystem_lib)
target_link_libraries(dispatcher_lib request_handler_lib factory_request_handler_lib config_parser_lib cppmarkdown)
target_link_libraries(session_lib request_handler_lib request_parser_lib logging_lib dispatcher_lib)
target_link_libraries(server filesystem_lib request_handler_lib server_lib session_lib config_parser_lib logging_lib dispatcher_lib Boost::system Threads::Threads Boost::log_setup Boost::log ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY})


add_executable(dispatcher_test tests/dispatcher_test.cc)
target_link_libraries(dispatcher_test request_parser_lib dispatcher_lib request_handler_lib factory_request_handler_lib logging_lib gtest_main Boost::system Boost::log_setup Boost::log)
gtest_discover_tests(dispatcher_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)


#add_executable(server_echo_test tests/echo_server_test.cc)
#target_link_libraries(server_echo_test server_lib session_lib reply_lib config_parser_lib logging_lib gtest_main)

add_executable(crud_reply_test tests/crud_reply_test.cc)
#target_link_libraries(crud_reply_test request_parser_lib dispatcher_lib crud_request_handler_lib factory_request_handler_lib logging_lib gtest_main gmock_main Boost::system Boost::log_setup Boost::log)
target_link_libraries(crud_reply_test request_parser_lib dispatcher_lib request_handler_lib factory_request_handler_lib logging_lib gtest_main gmock_main Boost::system Boost::log_setup Boost::log)
gtest_discover_tests(crud_reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

#add_executable(reply_test tests/file_reply_test.cc tests/echo_reply_test.cc tests/http_server_session_test.cc tests/config_parser_test.cc tests/error_reply_test.cc)
#target_link_libraries(reply_test session_lib reply_lib server_lib config_parser_lib logging_lib gtest_main Boost::system Boost::log_setup Boost::log)

add_executable(request_parser_test tests/malformed_test.cc)
target_link_libraries(request_parser_test request_parser_lib gtest_main gmock_main logging_lib Boost::system Boost::log_setup Boost::log)
gtest_discover_tests(request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# TODO(!): Update with test binary
#gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# TODO(!): Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
#add_test(NAME CRUDIntegrationTest COMMAND python3 integration_test.py 8080 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/)
generate_coverage_report(TARGETS server server_lib session_lib request_handler_lib config_parser_lib logging_lib 
  TESTS crud_reply_test dispatcher_test request_parser_test)

#add_test(NAME integration_test
#  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_test.sh
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_test(NAME multi_thread_test COMMAND python3 multi_threading_server_test.py 8080 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/)