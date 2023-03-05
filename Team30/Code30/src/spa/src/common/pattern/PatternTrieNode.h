#pragma once

#include <memory>
#include <string>
#include <map>

using std::unique_ptr, std::map;

typedef uint16_t SymbolIdent;

const SymbolIdent TRIE_INVALID_SYMBOL = 65535;
const SymbolIdent TRIE_END_NODE = 65534;

class PatternTrieNode;
typedef unique_ptr<PatternTrieNode> PatternTrieNodePtr;

class PatternTrieNode {
 private:
  map<uint16_t, PatternTrieNodePtr> next;

 public:
  PatternTrieNode();
  PatternTrieNode* addNext(uint16_t nodeId);
  PatternTrieNode* traverse(uint16_t nodeId);

  void addEnd();
  bool isEnd();
};
