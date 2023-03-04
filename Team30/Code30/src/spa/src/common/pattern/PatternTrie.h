#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "common/ast/IAST.h"
#include "ExpressionSequence.h"
#include "PatternTrieNode.h"

using std::unique_ptr, std::shared_ptr, std::unordered_map, std::string;

typedef unordered_map<string, SymbolIdent> TrieSymbolTable;
typedef unique_ptr<TrieSymbolTable> TrieSymbolTablePtr;

class PatternTrie {
 public:
  explicit PatternTrie(PatternTrieNodePtr trieRoot,
                       TrieSymbolTablePtr symbolTable,
                       int longestPathCount);
  bool isMatchFull(ExpressionSequence* sequence);
  bool isMatchPartial(ExpressionSequence* sequence);

 private:
  PatternTrieNodePtr root;
  TrieSymbolTablePtr symbolTable;
  int longestPathCount;

  SymbolIdent lookupSymbol(const string &symbol);
  bool isValidPostfix(ExpressionSequence *sequence);
};

typedef unique_ptr<PatternTrie> PatternTriePtr;
typedef shared_ptr<PatternTrie> PatternTrieSPtr;
