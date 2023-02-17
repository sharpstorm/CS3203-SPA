#pragma once

#include "PQLSuchThatClauseContext.h"
#include "../../../../clauses/UsesClause.h"
#include "../../ref_extractor/PQLEntityRefExtractor.h"
#include "../../ref_extractor/PQLAnyRefExtractor.h"

class PQLUsesClauseContext: public PQLSuchThatClauseContext {
 public:
  void parse(QueryTokenParseState* parserState) {
    parseArgs<
        UsesClause,
        PQLAnyRefExtractor,
        PQLEntityRefExtractor>(parserState);
  }
};
