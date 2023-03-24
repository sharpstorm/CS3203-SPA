#include "PatternTrie.h"

#include <utility>
#include <string>

using std::to_string;

PatternTrie::PatternTrie(PatternTrieNodePtr trieRoot,
                         TrieSymbolTablePtr symbolTable,
                         int longestPathCount):
    root(std::move(trieRoot)),
    symbolTable(std::move(symbolTable)),
    longestPathCount(longestPathCount) {}

// TODO Use uint16
bool PatternTrie::isMatchFull(ExpressionSequence *sequence) {
  if (root == nullptr) {
    return false;
  }

  if (sequence->size() != longestPathCount) {
    return false;
  }

  return isValidPostfix(sequence);
}

// TODO Use uint16
bool PatternTrie::isMatchPartial(ExpressionSequence *sequence) {
  if (root == nullptr) {
    return false;
  }

  return isValidPostfix(sequence);
}

// TODO Use uint16
bool PatternTrie::isValidPostfix(ExpressionSequence *sequence) {
  PatternTrieNode* curNode = root.get();
  for (int i = 0; i < sequence->size(); i++) {
    // Ask PKB for value
    SymbolIdent symbol = lookupSymbol(sequence->at(i));
    curNode = curNode->traverse(symbol);
    if (curNode == nullptr) {
      return false;
    }
  }

  return curNode->isEnd();
}

SymbolIdent PatternTrie::lookupSymbol(const string &symbol) {
  auto result = symbolTable->find(symbol);
  if (result == symbolTable->end()) {
    return TRIE_INVALID_SYMBOL;
  }

  return result->second;
}
