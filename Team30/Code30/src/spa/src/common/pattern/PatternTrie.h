#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "common/ast/IAST.h"
#include "ExpressionSequence.h"
#include "PatternTrieNode.h"

using std::unique_ptr, std::shared_ptr, std::unordered_map, std::string;

class PatternTrie {
 public:
  explicit PatternTrie(PatternTrieNodePtr trieRoot,
                       int longestPathCount);
  bool isMatchFull(const ExpressionSequence* sequence) const;
  bool isMatchPartial(const ExpressionSequence* sequence) const;

 private:
  PatternTrieNodePtr root;
  int longestPathCount;

  bool isValidPostfix(const ExpressionSequence *sequence) const;
};

typedef unique_ptr<PatternTrie> PatternTriePtr;
typedef shared_ptr<PatternTrie> PatternTrieSPtr;
