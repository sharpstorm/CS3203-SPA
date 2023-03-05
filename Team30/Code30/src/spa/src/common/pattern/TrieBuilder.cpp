#include <memory>
#include <utility>

#include "TrieBuilder.h"

using std::make_unique;

TrieBuilder::TrieBuilder(IASTNodePtr astRoot):
    astRoot(astRoot),
    symTable(make_unique<TrieSymbolTable>()),
    rootNode(make_unique<PatternTrieNode>()),
    nodeCount(0),
    symbolIdCounter(0) {
}

PatternTriePtr TrieBuilder::build() {
  walkAST(astRoot.get());

  return make_unique<PatternTrie>(std::move(rootNode),
                                  std::move(symTable),
                                  nodeCount);
}

TrieBuilder::BuildState TrieBuilder::walkAST(IASTNode* node) {
  nodeCount++;

  if (node->getChildCount() == 0) {
    SymbolIdent symId = registerLeaf(node->getValue());
    ownedProcessingNodes.push_back(make_unique<ProcessingNode>(nullptr, symId));
    return TrieBuilder::BuildState {
        rootNode->traverse(symId),
        ownedProcessingNodes.back().get(),
        ownedProcessingNodes.back().get(),
    };
  }

  TrieBuilder::BuildState leftChild = walkAST(node->getChild(0).get());
  TrieBuilder::BuildState rightChild = walkAST(node->getChild(1).get());

  PatternTrieNode* curNode = leftChild.node;
  leftChild.postfixTail->next = rightChild.postfixHead;

  ProcessingNode* curPNode = rightChild.postfixHead;
  while (curPNode != nullptr) {
    curNode = curNode->addNext(curPNode->ident);
    curPNode = curPNode->next;
  }

  SymbolIdent newSymId = registerSymbol(node->getValue());
  ownedProcessingNodes.push_back(
      make_unique<ProcessingNode>(nullptr, newSymId));
  ProcessingNode* newPNode = ownedProcessingNodes.back().get();
  curNode = curNode->addNext(newSymId);
  curNode->addEnd();

  rightChild.postfixTail->next = newPNode;

  return TrieBuilder::BuildState{
    curNode,
    leftChild.postfixHead,
    newPNode,
  };
}

SymbolIdent TrieBuilder::registerLeaf(const string &symbol) {
  SymbolIdent symId = registerSymbol(symbol);

  // Newly registered
  if (symId == symbolIdCounter - 1) {
    PatternTrieNode* newNode = rootNode->addNext(symId);
    newNode->addEnd();
  }

  return symId;
}

SymbolIdent TrieBuilder::registerSymbol(const string &symbol) {
  auto it = symTable->find(symbol);
  if (it != symTable->end()) {
    return it->second;
  }

  SymbolIdent symId = symbolIdCounter;
  symbolIdCounter++;

  symTable->emplace(symbol, symId);
  return symId;
}
