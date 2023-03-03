#pragma once
#include <string>
#include "pkb/writers/PkbWriter.h"
#include "SourceParser.h"

using std::string;

class SpDriver {
 public:
  void parseSource(string input, PkbWriter* pkbWriter);
  AST parseExpression(string expression);

 private:
  SourceParser parser;
};
