#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "PQLQuerySynonym.h"

using std::string, std::unordered_set;

typedef PQLQuerySynonym** PQLQuerySynonymProxy;

class VariableTable {
 private:
  unordered_map<string, PQLQuerySynonym*> proxyArray;
  vector<PQLQuerySynonym> declaredSynonyms;
 public:
  VariableTable();
  explicit VariableTable(unordered_map<string, PQLQuerySynonym> map);
  int size();
  void add(const PQLSynonymName &name, PQLQuerySynonym syn);
  void finalizeTable();
  PQLQuerySynonymProxy getProxyFor(const PQLSynonymName &name);
  vector<PQLQuerySynonym> getDeclaredSynonyms();
  unordered_map<string, PQLQuerySynonym*>* getProxyArray();
  PQLQuerySynonym* find(const PQLSynonymName &name);
  unordered_set<PQLQuerySynonym*> getReferredSynonyms();
};

typedef unique_ptr<VariableTable> VariableTablePtr;
