#ifndef FILESYSTEMINTERFACE_H
#define FILESYSTEMINTERFACE_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <cstdlib>

#include "logging.h"

using namespace std;

class FileSystemInterface
{
  public:
    /// Construct a FileSystem with a base directory.
    FileSystemInterface() {}
    virtual ~FileSystemInterface() {}
    virtual bool readFile(const string path, string& file_content) = 0;
    virtual bool writeFile(const string path, const string data) = 0;
    virtual bool deleteFile(const string path) = 0;
    virtual bool listFiles(const string path, string& file_list) = 0;

};

#endif // FILESYSTEMINTERFACE_H