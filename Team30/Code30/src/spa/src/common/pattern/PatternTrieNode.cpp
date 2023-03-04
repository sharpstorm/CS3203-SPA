#include "PatternTrieNode.h"

#include <utility>

PatternTrieNode::PatternTrieNode() {
}

PatternTrieNode* PatternTrieNode::addNext(uint16_t nodeId) {
  auto node = next.find(nodeId);
  if (node != next.end()) {
    return node->second.get();
  }

  PatternTrieNode* newNode = new PatternTrieNode();
  next.emplace(nodeId, PatternTrieNodePtr(newNode));

  return newNode;
}

PatternTrieNode *PatternTrieNode::traverse(uint16_t nodeId) {
  auto node = next.find(nodeId);
  if (node == next.end()) {
    return nullptr;
  }
  return node->second.get();
}

bool PatternTrieNode::isEnd() {
  return next.find(TRIE_END_NODE) != next.end();
}

void PatternTrieNode::addEnd() {
  next.emplace(TRIE_END_NODE, nullptr);
}
