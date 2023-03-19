#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "PQLQuerySynonym.h"
#include "PQLQuerySynonymProxy.h"

typedef unordered_map<string, PQLQuerySynonymProxy> ProxyMap;

using std::string, std::unordered_set;

class VariableTable {
 private:
  ProxyMap proxyMap;
  vector<PQLQuerySynonym> declaredSynonyms;
  vector<PQLQuerySynonym*> rawProxy;
 public:
  VariableTable();
  explicit VariableTable(unordered_map<string, PQLQuerySynonym> map);
  int size();
  void add(const PQLSynonymName &name, PQLQuerySynonym syn);
  void finalizeTable();

  ProxyMap* getProxyMap();
  unordered_set<PQLSynonymName> getReferredSynonyms();
  PQLQuerySynonymProxy* find(const PQLSynonymName &name);
};

typedef unique_ptr<VariableTable> VariableTablePtr;
