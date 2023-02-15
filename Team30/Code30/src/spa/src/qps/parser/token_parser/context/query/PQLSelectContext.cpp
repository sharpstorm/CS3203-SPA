#include "PQLSelectContext.h"
#include "qps/errors/QPSParserSemanticError.h"

void PQLSelectContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  PQLToken* currentToken = parserState->expectSynName();
  PQLQuerySynonym* synonym = parserState->getQueryBuilder()
      ->getVariable(currentToken->getData());

  if (synonym == nullptr) {
    parserState->setSemanticError(QPS_PARSER_ERR_UNKNOWN_SELECT);
    return;
  }

  parserState->getQueryBuilder()->setResultVariable(
      synonym->getType(),
      synonym->getName());
}
