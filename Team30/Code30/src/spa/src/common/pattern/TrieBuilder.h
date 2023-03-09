#pragma once

#include <vector>
#include <string>
#include <memory>

#include "PatternTrie.h"

using std::vector, std::string, std::unique_ptr;

class TrieBuilder {
 public:
  explicit TrieBuilder(IASTNodePtr astRoot);
  PatternTriePtr build();

 private:
  struct ProcessingNode {
    ProcessingNode* next;
    SymbolIdent ident;
    ProcessingNode(ProcessingNode* next, SymbolIdent ident):
        next(next), ident(ident) {}
  };

  typedef unique_ptr<ProcessingNode> ProcessingNodePtr;

  struct BuildState {
    PatternTrieNode* node;
    ProcessingNode* postfixHead;
    ProcessingNode* postfixTail;
  };

  IASTNodePtr astRoot;
  TrieSymbolTablePtr symTable;
  PatternTrieNodePtr rootNode;
  int nodeCount;
  SymbolIdent symbolIdCounter;
  vector<ProcessingNodePtr> ownedProcessingNodes;

  SymbolIdent registerSymbol(const string &symbol);
  SymbolIdent registerLeaf(const string &symbol);
  BuildState walkAST(IASTNode* node);
};