#include "PQLConditionalParser.h"

PQLConditionalParser::PQLConditionalParser(ISourceExpressionParser *exprParser):
    patternParser(exprParser) {}

void PQLConditionalParser::parse(QueryTokenParseState* parserState,
                                 QueryBuilder *queryBuilder) {
  IPQLParser* context = getParseContext(parserState->getCurrentTokenType());
  while (context != nullptr) {
    context->parse(parserState, queryBuilder);
    context = getParseContext(parserState->getCurrentTokenType());
  }
}

IPQLParser *PQLConditionalParser::getParseContext(PQLTokenType type) {
  switch (type) {
    case PQL_TOKEN_SUCH:
      return &suchThatParser;
    case PQL_TOKEN_PATTERN:
      return &patternParser;
    default:
      return nullptr;
  }
}
