#pragma once

#include <memory>
#include <string>

#include "Clause.h"
#include "qps/common/AttributedSynonym.h"

using std::unique_ptr, std::string;

class WithSelectClause : public Clause {
  AttributedSynonym attrSyn;
  EntityValue entVal;

 public:
  WithSelectClause(AttributedSynonym aSyn, EntityValue entV);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                             OverrideTable* table) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
};

typedef unique_ptr<WithSelectClause> WithSelectClausePtr;
