#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "PQLQuerySynonym.h"
#include "PQLQuerySynonymProxy.h"

using std::unordered_map, std::vector, std::unique_ptr;

typedef unordered_map<PQLSynonymName, PQLQuerySynonymProxy> ProxyMap;
typedef unordered_map<PQLSynonymName, PQLQuerySynonym> SynonymMap;

class VariableTable {
 private:
  ProxyMap proxyMap;
  vector<PQLQuerySynonym> declaredSynonyms;
  vector<PQLQuerySynonym *> rawProxy;

 public:
  VariableTable();
  explicit VariableTable(const SynonymMap &map);
  int size() const;
  void add(const PQLSynonymName &name, const PQLQuerySynonym &syn);
  void finalizeTable();

  ProxyMap *getProxyMap();
  PQLSynonymNameSet getReferredSynonyms() const;
  PQLQuerySynonymProxy *const find(const PQLSynonymName &name);
};

typedef unique_ptr<VariableTable> VariableTablePtr;
