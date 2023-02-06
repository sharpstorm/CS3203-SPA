#pragma once

#include <string>
#include "ISourceParser.h"

using std::string;

class SourceParser : public ISourceParser {
 public:
  AST parseSource(string input);
};

