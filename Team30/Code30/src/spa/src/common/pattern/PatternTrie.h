#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "common/ast/IAST.h"
#include "ExpressionSequence.h"
#include "PatternTrieNode.h"

using std::unique_ptr, std::shared_ptr, std::unordered_map, std::string;

// z + 3
// 1. z + x -> z x +
// 2. Cannot identify the index of variables in line <- Bit magic
// Might need PKB Querier to get the symbol numbers

// type - uint16
// 0 - 32767 -> Variable
// 32768 - 65528 -> Constant
// 65535 - INVALID
// 65534 - END
// 65529 - 65533 - Special

class PatternTrie {
 public:
  explicit PatternTrie(PatternTrieNodePtr trieRoot,
                       int longestPathCount);
  bool isMatchFull(ExpressionSequence* sequence);
  bool isMatchPartial(ExpressionSequence* sequence);

 private:
  PatternTrieNodePtr root;
  int longestPathCount;

  SymbolIdent lookupSymbol(const string &symbol);
  bool isValidPostfix(ExpressionSequence *sequence);
};

typedef unique_ptr<PatternTrie> PatternTriePtr;
typedef shared_ptr<PatternTrie> PatternTrieSPtr;
