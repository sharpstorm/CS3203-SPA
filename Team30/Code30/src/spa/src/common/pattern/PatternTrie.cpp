#include "PatternTrie.h"

#include <utility>
#include <string>

using std::to_string;

PatternTrie::PatternTrie(PatternTrieNodePtr trieRoot,
                         int longestPathCount):
    root(std::move(trieRoot)),
    longestPathCount(longestPathCount) {}

bool PatternTrie::isMatchFull(ExpressionSequence *sequence) {
  if (root == nullptr) {
    return false;
  }

  if (sequence->size() != longestPathCount) {
    return false;
  }

  return isValidPostfix(sequence);
}

bool PatternTrie::isMatchPartial(ExpressionSequence *sequence) {
  if (root == nullptr) {
    return false;
  }

  return isValidPostfix(sequence);
}

bool PatternTrie::isValidPostfix(ExpressionSequence *sequence) {
  PatternTrieNode* curNode = root.get();
  for (int i = 0; i < sequence->size(); i++) {
    curNode = curNode->traverse(sequence->at(i));
    if (curNode == nullptr) {
      return false;
    }
  }

  return curNode->isEnd();
}
