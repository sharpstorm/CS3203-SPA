#pragma once

#include <memory>

#include "SynonymUFDS.h"

using std::unique_ptr;

class VariableTableProxyBuilder {
 private:
  VariableTable* table;
  SynonymUFDS ufds;

 public:
  explicit VariableTableProxyBuilder(VariableTable* varTable) :
      table(varTable), ufds(SynonymUFDS(varTable)) {}
  void joinSynonyms(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2);
  void build();
  VariableTable* getTable();
};

typedef unique_ptr<VariableTableProxyBuilder> VariableTableProxyBuilderPtr;
