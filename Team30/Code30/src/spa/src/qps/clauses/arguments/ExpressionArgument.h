#pragma once

#include <string>
#include <memory>
#include <vector>

#include "common/pattern/PatternTrieNode.h"

using std::string, std::unique_ptr, std::vector;

typedef vector<string> ExpressionSequence;
typedef unique_ptr<ExpressionSequence> ExpressionSequencePtr;

class ExpressionArgument {
 public:
  ExpressionArgument();
  ExpressionArgument(ExpressionSequencePtr expression, bool isPartial);
  bool isWildcard();
  bool allowsPartial();
  ExpressionSequence* getSequence();

 private:
  ExpressionSequencePtr expressionSequence;
  bool isPartial;
};

typedef unique_ptr<ExpressionArgument> ExpressionArgumentPtr;
