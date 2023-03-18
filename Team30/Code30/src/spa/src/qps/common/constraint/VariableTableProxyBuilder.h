#pragma
#include "SynonymUFDS.h"

class VariableTableProxyBuilder {
 private:
  VariableTable* table;
  SynonymUFDS ufds;

 public:
  VariableTableProxyBuilder(VariableTable* varTable) :
      table(varTable),ufds(SynonymUFDS(varTable)) {}
  void joinSynonyms(PQLQuerySynonym* syn1, PQLQuerySynonym* syn2);
  void build();
  VariableTable* getTable();
};