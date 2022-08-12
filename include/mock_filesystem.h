#ifndef MOCK_FILESYSTEM_H
#define MOCK_FILESYSTEM_H

#include "gmock/gmock.h"
#include "filesystem_interface.h"

using namespace std;

class MockFileSystem : public FileSystemInterface
{
  public:

    MockFileSystem() {}
    MOCK_METHOD(bool, readFile, (const string path, string& file_content), (override));
    MOCK_METHOD(bool, writeFile, (const string path, const string data), (override));
    MOCK_METHOD(bool, deleteFile, (string path), (override));
    MOCK_METHOD(bool, listFiles, (string path, string& file_list), (override));
};

#endif // MOCK_FILESYSTEM_H