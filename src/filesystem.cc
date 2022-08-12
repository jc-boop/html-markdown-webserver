#include "filesystem.h"

using namespace std;
namespace fs = boost::filesystem;

bool FileSystem::readFile(const string path, string& file_content) {
  log_debug("readFile file name: " + path);
  
  string file_temp = "";
  fstream req_file;

  req_file.open(path, ios::in);

  if(!req_file || path == "") {   // file not found
    req_file.close();
    return false;
  } 

  string line_str = "";

  while(getline(req_file, line_str)) {
    file_temp += line_str + "\n";
  }

  file_content = file_temp;
  file_temp.clear();
  req_file.close();
  return true;
}

bool FileSystem::writeFile(const string path, const string data) {
  log_debug("writeFile file name: " + path);

  fstream req_file;
  size_t last_pos;
  size_t next_pos;
  string directory;
  string file;

  // Path processing to check for directory or file
  last_pos = path.find_last_of("/");
  if (last_pos != string::npos) {
    // starts with "/"
    if (last_pos == 0) {
      directory = "";
      file = path;
    }
    // trailing
    else if (last_pos == path.length() - 1) {
      string without_trailing = path.substr(0, last_pos - 1);
      next_pos = without_trailing.find_last_of("/");
      if (next_pos != string::npos) {
        directory = path.substr(0, next_pos);
        file = path.substr(next_pos + 1, last_pos - (next_pos + 1));
      }
      else {
        directory = "";
        file = without_trailing;
      }
    }
    else {
      directory = path.substr(0, last_pos);
      file = path.substr(last_pos + 1);
    }
  }
  else {
    directory = "";
    file = path;
  }

  if (file == "") {
    log_debug("Invalid path: " + path);
    return false;
  }
  
  log_debug("directory: " + directory + ", file: " + file);
  string new_path = directory + "/" + file;

  // if directory exists in path
  if (directory != "") {
    const fs::path dir(directory);
    bool dir_exists = fs::exists(dir);
    if(!dir_exists){
      log_debug("Creating directory: " + directory);
      if (fs::create_directories(dir))
          log_debug("directory " + directory + " created");
    }
  }
  
  req_file.open(new_path, fstream::out);

  if(!req_file || new_path == "") {   // file not found
    log_debug("File nonexistent: " + path);
    return false;
  } 

  log_debug("File opened: " + new_path);
  req_file << data;
  req_file.close();

  return true;
}


bool FileSystem::deleteFile(const string path) {
  log_debug("deleteFile file name: " + path);

  fstream req_file;
  req_file.open(path, ios::out);

  if(!req_file || path == "") {   // file not found
    return false;
  } 

  int success = remove(path.c_str()); 
  //remove returns 0 on success 
  if (success != 0) {
    log_debug("could not delete file ");
    return false;
  }
  return true;
}


bool FileSystem::listFiles(const string path, string& file_list) {
  log_debug("listFiles file name: " + path);
  ostringstream strm;

  DIR* dr = opendir(path.c_str());
  struct dirent *en;
  
  if (dr) {
    while ((en = readdir(dr)) != NULL) {
      if (en->d_name[0] != '.')   // ensures that '.' and '..' are not included in our filelist
        strm << en->d_name << ",";
    }
    closedir(dr);
  }
  else {
    file_list = "";
    return false;
  }

  file_list = strm.str();
  return true;
}


