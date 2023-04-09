#pragma once

#include <vector>
#include <string>
#include <memory>

#include "PatternTrie.h"
#include "pkb/writers/PkbWriter.h"

using std::vector, std::string, std::unique_ptr;

class TrieBuilder {
 public:
  explicit TrieBuilder(IASTNode *astRoot, PkbWriter *pkbWriter);
  PatternTriePtr build();

  static SymbolIdent writeVariable(IASTNode *node, PkbWriter *writer);
  static SymbolIdent writeConstant(IASTNode *node, PkbWriter *writer);

 private:
  struct ProcessingNode {
    ProcessingNode *next;
    SymbolIdent ident;
    ProcessingNode(ProcessingNode *next, SymbolIdent ident) :
        next(next), ident(ident) {}
  };

  typedef unique_ptr<ProcessingNode> ProcessingNodePtr;

  struct BuildState {
    PatternTrieNode *node;
    ProcessingNode *postfixHead;
    ProcessingNode *postfixTail;
  };

  IASTNode *astRoot;
  PatternTrieNodePtr rootNode;
  int nodeCount;
  vector<ProcessingNodePtr> ownedProcessingNodes;
  PkbWriter *pkbWriter;

  SymbolIdent registerSymbol(IASTNode *node);
  SymbolIdent registerLeaf(IASTNode *node);
  BuildState walkAST(IASTNode *node);
};
