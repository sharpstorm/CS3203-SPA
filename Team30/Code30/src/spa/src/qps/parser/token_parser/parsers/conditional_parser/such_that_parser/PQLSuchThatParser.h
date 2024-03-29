#pragma once

#include "../../IPQLParser.h"

#include "contexts/PQLSuchThatClauseTypes.h"

class PQLSuchThatParser : public IPQLParser {
 public:
  void parse(QueryTokenParseState *parserState,
             QueryBuilder *queryBuilder) override;

 private:
  PQLFollowsClauseContext followsContext;
  PQLParentClauseContext parentContext;
  PQLUsesClauseContext usesContext;
  PQLModifiesClauseContext modifiesContext;
  PQLCallsClauseContext callsContext;
  PQLNextClauseContext nextContext;
  PQLAffectsClauseContext affectsContext;

  IPQLSuchThatClauseContext *getContext(PQLTokenType type);
};
