#pragma once

#include <memory>

#include "SynonymUFDS.h"
#include "OverrideTable.h"

using std::unique_ptr;

class SynonymProxyBuilder {
 private:
  VariableTable* variableTable;
  SynonymUFDS ufds;

 public:
  explicit SynonymProxyBuilder(VariableTable* varTable);
  bool joinSynonyms(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2);
  void build();
  bool resolveOverrideMerging(OverrideTable* overrideTable);

  VariableTable* getTable() const;
};

typedef unique_ptr<SynonymProxyBuilder> SynonymProxyBuilderPtr;
