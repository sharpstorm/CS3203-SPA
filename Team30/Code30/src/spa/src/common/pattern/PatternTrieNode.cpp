#include "PatternTrieNode.h"

PatternTrieNode::PatternTrieNode() {
}

void PatternTrieNode::addNext(string nodeId, PatternTrieNodePtr nextPtr) {
  next.emplace(nodeId, std::move(nextPtr));
}

bool PatternTrieNode::hasNext(string nodeId) {
  return next.find(nodeId) != next.end();
}
