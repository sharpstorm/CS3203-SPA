#include <memory>
#include <utility>

#include "TrieBuilder.h"

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
  switch (node->getType()) {
    case ASTNODE_CONSTANT:
      return pkbWriter->addConstant(node->getValue()) | TRIE_CONST_MASK;
    case ASTNODE_VARIABLE:
      return pkbWriter->addVariable(node->getValue());
    case ASTNODE_PLUS:
      return TRIE_PLUS;
    case ASTNODE_MINUS:
      return TRIE_MINUS;
    case ASTNODE_TIMES:
      return TRIE_TIMES;
    case ASTNODE_DIV:
      return TRIE_DIV;
    case ASTNODE_MOD:
      return TRIE_MOD;
    default:
      return TRIE_INVALID_SYMBOL;
  }
}
