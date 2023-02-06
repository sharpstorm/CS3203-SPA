#pragma once

#include <string>

using std::string;

class FileReader {
 public:
  FileReader();
  string readFromFile(string fileName);
};
