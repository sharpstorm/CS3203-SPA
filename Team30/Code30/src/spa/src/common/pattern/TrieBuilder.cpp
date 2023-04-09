#include <memory>
#include <utility>

#include "TrieBuilder.h"
#include "PatternNodeConverter.h"

using std::make_unique;

TrieBuilder::TrieBuilder(IASTNode *astRoot, PkbWriter *pkbWriter) :
    astRoot(astRoot),
    rootNode(make_unique<PatternTrieNode>()),
    nodeCount(0),
    pkbWriter(pkbWriter) {
}

PatternTriePtr TrieBuilder::build() {
  walkAST(astRoot);

  return make_unique<PatternTrie>(std::move(rootNode),
                                  nodeCount);
}

TrieBuilder::BuildState TrieBuilder::walkAST(IASTNode *node) {
  nodeCount++;

  if (node->getChildCount() == 0) {
    SymbolIdent symId = registerLeaf(node);
    ownedProcessingNodes.push_back(make_unique<ProcessingNode>(nullptr, symId));
    return TrieBuilder::BuildState{
        rootNode->traverse(symId),
        ownedProcessingNodes.back().get(),
        ownedProcessingNodes.back().get(),
    };
  }

  TrieBuilder::BuildState leftChild = walkAST(node->getChild(0));
  TrieBuilder::BuildState rightChild = walkAST(node->getChild(1));

  PatternTrieNode *curNode = leftChild.node;
  leftChild.postfixTail->next = rightChild.postfixHead;

  TrieBuilder::ProcessingNode *curPNode = rightChild.postfixHead;
  while (curPNode != nullptr) {
    curNode = curNode->addNext(curPNode->ident);
    curPNode = curPNode->next;
  }

  SymbolIdent newSymId = registerSymbol(node);
  ownedProcessingNodes.push_back(make_unique<ProcessingNode>(nullptr,
                                                             newSymId));
  TrieBuilder::ProcessingNode *newPNode = ownedProcessingNodes.back().get();
  curNode = curNode->addNext(newSymId);
  curNode->addEnd();

  rightChild.postfixTail->next = newPNode;

  return TrieBuilder::BuildState{
      curNode,
      leftChild.postfixHead,
      newPNode,
  };
}

SymbolIdent TrieBuilder::registerLeaf(IASTNode *node) {
  SymbolIdent symId = registerSymbol(node);

  // Newly registered
  if (!rootNode->hasChild(symId)) {
    PatternTrieNode *newNode = rootNode->addNext(symId);
    newNode->addEnd();
  }

  return symId;
}

SymbolIdent TrieBuilder::registerSymbol(IASTNode *node) {
  return PatternNodeConverter<PkbWriter>::indexFromNode<
      TrieBuilder::writeVariable,
      TrieBuilder::writeConstant>(node, pkbWriter);
}

SymbolIdent TrieBuilder::writeVariable(IASTNode *node, PkbWriter *writer) {
  return writer->addVariable(node->getValue());
}

SymbolIdent TrieBuilder::writeConstant(IASTNode *node, PkbWriter *writer) {
  return writer->addConstant(node->getValue());
}
