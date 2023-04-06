#include "PatternTrie.h"

#include <utility>
#include <string>

using std::to_string;

PatternTrie::PatternTrie(PatternTrieNodePtr trieRoot,
                         int longestPathCount):
    root(std::move(trieRoot)),
    longestPathCount(longestPathCount) {}

bool PatternTrie::isMatchFull(const ExpressionSequence *sequence) const {
  if (root == nullptr) {
    return false;
  }

  if (sequence->size() != longestPathCount) {
    return false;
  }

  return isValidPostfix(sequence);
}

bool PatternTrie::isMatchPartial(const ExpressionSequence *sequence) const {
  if (root == nullptr) {
    return false;
  }

  return isValidPostfix(sequence);
}

bool PatternTrie::isValidPostfix(const ExpressionSequence *sequence) const {
  PatternTrieNode* curNode = root.get();
  for (int i = 0; i < sequence->size(); i++) {
    curNode = curNode->traverse(sequence->at(i));
    if (curNode == nullptr) {
      return false;
    }
  }

  return curNode->isEnd();
}
