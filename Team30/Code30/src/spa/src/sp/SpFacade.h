#pragma once
#include <string>
#include "ISp.h"
#include "SourceParser.h"
#include "FileReader.h"

using std::string;

class SpFacade : public ISp {
 public:
  void parseSource(string input);

 private:
  FileReader fileReader;
  SourceParser parser;
};
