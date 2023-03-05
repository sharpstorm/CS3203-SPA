#pragma once

#include <memory>
#include <string>
#include <unordered_map>

using std::unique_ptr, std::unordered_map, std::string;

class PatternTrieNode;
typedef unique_ptr<PatternTrieNode> PatternTrieNodePtr;

class PatternTrieNode {
 private:
  unordered_map<string, PatternTrieNodePtr> next;

 public:
  PatternTrieNode();
  void addNext(string nodeId, PatternTrieNodePtr nextPtr);
  bool hasNext(string nodeId);
};
