#pragma once
#include <string>

#include "ISp.h"
#include "ISpExpressionParser.h"
#include "SourceParser.h"
#include "FileReader.h"
#include "SpDriver.h"

using std::string;

class SpFacade : public ISp, public ISpExpressionParser {
 public:
  void parseSource(string input, PkbWriter* pkbWriter);
  AST parseExpression(string expression);

 private:
  FileReader fileReader;
  SpDriver spDriver;
};
