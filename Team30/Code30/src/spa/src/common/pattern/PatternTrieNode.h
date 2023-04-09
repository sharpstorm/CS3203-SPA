#pragma once

#include <memory>
#include <string>
#include <map>

using std::unique_ptr, std::map;

// uint_16 is used to save 2 bytes per node reference
typedef uint16_t SymbolIdent;

const SymbolIdent TRIE_CONST_MASK = 0x8000;
const SymbolIdent TRIE_PLUS = 65529;
const SymbolIdent TRIE_MINUS = 65530;
const SymbolIdent TRIE_TIMES = 65531;
const SymbolIdent TRIE_DIV = 65532;
const SymbolIdent TRIE_MOD = 65533;
const SymbolIdent TRIE_INVALID_SYMBOL = 65535;
const SymbolIdent TRIE_END_NODE = 65534;

class PatternTrieNode;
typedef unique_ptr<PatternTrieNode> PatternTrieNodePtr;

class PatternTrieNode {
 private:
  map<SymbolIdent, PatternTrieNodePtr> next;

 public:
  PatternTrieNode();
  PatternTrieNode *addNext(SymbolIdent nodeId);
  PatternTrieNode *traverse(SymbolIdent nodeId);

  void addEnd();
  bool isEnd();
  bool hasChild(SymbolIdent id);
};
