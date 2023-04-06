#include <fstream>
#include <memory>
#include <string>
#include <utility>

#include "FileReader.h"
#include "sp/errors/SPError.h"

using std::ifstream, std::make_unique, std::string;

FileDataPtr FileReader::readFromFile(const FilePath &fileName) {
  FileDataPtr readData = make_unique<FileData>();

  ifstream file;
  file.open(fileName);
  if (!file) {
    throw SPError((fileName + " not found").c_str());
  }

  string line;
  line.reserve(1024);
  while (getline(file, line)) {
    readData->append(line + "\n");
  }

  file.close();

  return std::move(readData);
}
