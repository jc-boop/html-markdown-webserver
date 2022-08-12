#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "filesystem_interface.h"

using namespace std;

class FileSystem : public FileSystemInterface
{
  public:
    /// Construct a FileSystem with a base directory.
    FileSystem() {}
    ~FileSystem() {}

    // Returns true if file exists, else false
    bool readFile(const string path, string& file_content);

    // Returns true if file written to, else false
    bool writeFile(const string path, const string data);

    // Returns true if file exists and file deleted, else false
    bool deleteFile(const string path);

    // Returns true if dir exists, else false
    bool listFiles(const string path, string& file_list);

};

#endif // FILESYSTEM_H