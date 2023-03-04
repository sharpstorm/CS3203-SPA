#pragma once

#include <vector>
#include <string>

#include "PatternTrie.h"

using std::vector, std::string;

class TrieBuilder {
 public:
  explicit TrieBuilder(IASTNodePtr astRoot);
  PatternTriePtr build();

 private:
  IASTNodePtr astRoot;
  TrieSymbolTablePtr symTable;
  PatternTrieNodePtr rootNode;
  int nodeCount;
  SymbolIdent symbolIdCounter;

  struct BuildState {
    PatternTrieNode* node;
    vector<SymbolIdent> postfix;
  };

  SymbolIdent registerSymbol(const string &symbol);
  SymbolIdent registerLeaf(const string &symbol);
  BuildState walkAST(IASTNode* node);
};
