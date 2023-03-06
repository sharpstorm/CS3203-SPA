#pragma once

#include "../../IPQLParser.h"

#include "contexts/PQLFollowsClauseContext.h"
#include "contexts/PQLParentClauseContext.h"
#include "contexts/PQLUsesClauseContext.h"
#include "contexts/PQLModifiesClauseContext.h"
#include "contexts/PQLCallsClauseContext.h"

class PQLSuchThatParser: public IPQLParser {
 public:
  void parse(QueryTokenParseState* parserState, QueryBuilder *queryBuilder);

 private:
    PQLFollowsClauseContext followsContext;
    PQLParentClauseContext parentContext;
    PQLUsesClauseContext usesContext;
    PQLModifiesClauseContext modifiesContext;
    PQLCallsClauseContext callsContext;

  IPQLSuchThatClauseContext* getContext(PQLTokenType type);
};
