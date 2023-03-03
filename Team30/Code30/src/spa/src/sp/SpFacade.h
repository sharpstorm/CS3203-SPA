#pragma once
#include <string>

#include "ISp.h"
#include "common/parser/ISourceExpressionParser.h"
#include "SourceParser.h"
#include "FileReader.h"
#include "SpDriver.h"

using std::string;

class SpFacade : public ISp, public ISourceExpressionParser {
 public:
  void parseSource(string input, PkbWriter* pkbWriter);
  AST parseExpression(string expression);

 private:
  FileReader fileReader;
  SpDriver spDriver;
};
