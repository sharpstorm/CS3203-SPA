#pragma once

#include <string>
#include <memory>
#include "common/parser/ISourceExpressionParser.h"
#include "qps/clauses/arguments/ExpressionArgument.h"

using std::string, std::unique_ptr;

typedef string PatternString;

class IntermediateExpressionArgument {
 private:
  PatternString literalPattern;
  bool isPartial;

 public:
  IntermediateExpressionArgument(const PatternString &literalPattern,
                                 const bool isPartial);
  bool isWildcard();
  bool allowsPartial();
  IASTPtr parse(ISourceExpressionParser *parser);
};

typedef unique_ptr<IntermediateExpressionArgument>
    IntermediateExpressionArgumentPtr;
