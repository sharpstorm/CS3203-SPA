#pragma once

#include <memory>

#include "Clause.h"
#include "qps/common/AttributedSynonym.h"

using std::unique_ptr;

class WithSelectClause : public Clause {
  AttributedSynonym attrSyn;
//  PQLQueryResult* fetchStatementResults(PkbQueryHandler* pkbQueryHandler,
//                                        ClauseArgument* clauseArg,
//                                        OverrideTable* table);
//  PQLQueryResult* fetchEntityResults(PkbQueryHandler* pkbQueryHandler,
//                                     ClauseArgument* clauseArg,
//                                     OverrideTable* table);

 public:
  WithSelectClause(AttributedSynonym aSyn);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler, OverrideTable* table) override;
  bool validateArgTypes(VariableTable *variables) override;
  SynonymList getUsedSynonyms() override;
};

typedef unique_ptr<WithSelectClause> WithSelectClausePtr;
