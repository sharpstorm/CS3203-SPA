#pragma once

#include "PQLSuchThatClauseContext.h"
#include "../../../../clauses/ModifiesClause.h"
#include "../../ref_extractor/PQLEntityRefExtractor.h"
#include "../../ref_extractor/PQLAnyRefExtractor.h"

class PQLModifiesClauseContext: public PQLSuchThatClauseContext {
 public:
  void parse(QueryTokenParseState* parserState) {
    parseArgs<
        ModifiesClause,
        PQLAnyRefExtractor,
        PQLEntityRefExtractor>(parserState);
  }
};
