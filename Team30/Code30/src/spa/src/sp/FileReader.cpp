#include <string>
#include <fstream>
#include "FileReader.h"
#include "sp/errors/SPError.h"

using std::string, std::ifstream;

FileReader::FileReader() = default;

string FileReader::readFromFile(string fileName) {
  string programLines;
  string line;
  ifstream file;
  file.open(fileName);
  if (!file) {
    throw SPError((fileName + " not found").c_str());
  }
  while (getline(file, line)) {
    programLines += line + "\n";
  }
  file.close();
  return programLines;
}
