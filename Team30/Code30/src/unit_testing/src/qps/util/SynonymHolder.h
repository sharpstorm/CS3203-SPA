#pragma once

#include <memory>

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQuerySynonymProxy.h"
#include "qps/common/AttributedSynonym.h"

using std::unique_ptr, std::make_unique;

class SynonymHolder {
 private:
  vector<unique_ptr<PQLQuerySynonym>> rawSyns;
  vector<unique_ptr<PQLQuerySynonym*>> rawPtrLayer;
  unordered_map<PQLSynonymName, PQLQuerySynonymProxy> proxies;

 public:
  SynonymHolder();
  explicit SynonymHolder(vector<PQLQuerySynonym> syns);
  void addSyn(PQLSynonymName name, PQLSynonymType type);
  PQLQuerySynonymProxy getProxy(PQLSynonymName name);
};
