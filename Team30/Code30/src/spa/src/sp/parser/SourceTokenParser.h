#pragma once

#include <vector>
#include <memory>
#include "../common/SourceToken.h"
#include "GrammarContextProvider.h"
#include "sp/common/AST.h"

using std::shared_ptr;

class SourceTokenParser {
 public:
  SourceTokenParser();
  AST parse(vector<SourceToken>* tokens);
};
