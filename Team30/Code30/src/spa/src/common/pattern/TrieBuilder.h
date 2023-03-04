#pragma once

#include "PatternTrie.h"

class TrieBuilder {
 public:
  TrieBuilder(IASTNodePtr astRoot);
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
