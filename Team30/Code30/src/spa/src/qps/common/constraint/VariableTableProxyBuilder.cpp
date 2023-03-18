#include "VariableTableProxyBuilder.h"

void VariableTableProxyBuilder::joinSynonyms(PQLQuerySynonym *syn1,
                                             PQLQuerySynonym *syn2) {
  ufds.mergeSets(syn1, syn2);
}

void VariableTableProxyBuilder::VariableTableProxyBuilder::build() {
  for (auto i = table->getProxyArray()->begin();
  i != table->getProxyArray()->end(); i++) {
    i->second = ufds.findSet(i->second);
  }
}

VariableTable* VariableTableProxyBuilder::getTable() {
  return table;
}
