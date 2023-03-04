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
    return TrieBuilder::BuildState {
        rootNode->traverse(symId),
        {symId }
    };
  }

  TrieBuilder::BuildState leftChild = walkAST(node->getChild(0).get());
  TrieBuilder::BuildState rightChild = walkAST(node->getChild(1).get());

  vector<SymbolIdent> newPostfix;
  newPostfix.insert(
      newPostfix.end(),
      std::make_move_iterator(leftChild.postfix.begin()),
      std::make_move_iterator(leftChild.postfix.end())
  );

  PatternTrieNode* curNode = leftChild.node;
  for (int i = 0; i < rightChild.postfix.size(); i++) {
    SymbolIdent symId = rightChild.postfix.at(i);
    newPostfix.push_back(symId);
    curNode = curNode->addNext(symId);
  }

  SymbolIdent newSymId = registerSymbol(node->getValue());
  newPostfix.push_back(newSymId);
  curNode = curNode->addNext(newSymId);
  curNode->addEnd();

  return TrieBuilder::BuildState{
    curNode,
    newPostfix
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
