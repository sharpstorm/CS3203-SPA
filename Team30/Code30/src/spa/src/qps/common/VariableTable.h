#pragma once

#include <unordered_map>
#include <memory>

#include "qps/common/synonym/PQLQuerySynonym.h"
#include "qps/common/synonym/PQLQuerySynonymProxy.h"

using std::unordered_map, std::unique_ptr;

typedef unordered_map<PQLSynonymName, PQLQuerySynonymProxy> ProxyMap;
typedef unordered_map<PQLSynonymName, PQLQuerySynonym> SynonymMap;

class VariableTable {
 private:
  ProxyMap proxyMap;
  PQLQuerySynonymList declaredSynonyms;
  PQLQuerySynonymRefList rawProxy;

 public:
  VariableTable() = default;
  explicit VariableTable(const SynonymMap &map);
  int size() const;
  void add(const PQLQuerySynonym &syn);
  void finalizeTable();

  ProxyMap *getProxyMap();
  PQLQuerySynonymProxy *find(const PQLSynonymName &name);
};

typedef unique_ptr<VariableTable> VariableTablePtr;
