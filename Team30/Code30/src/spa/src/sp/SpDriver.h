#pragma once
#include <string>
#include "pkb/writers/PkbWriter.h"
#include "SourceParser.h"
#include "common/parser/ISourceExpressionParser.h"

using std::string;

class SpDriver : public ISourceExpressionParser {
 public:
  void parseSource(string input, PkbWriter* pkbWriter);
  IASTPtr parseExpression(string expression);

 private:
  SourceParser parser;
};
