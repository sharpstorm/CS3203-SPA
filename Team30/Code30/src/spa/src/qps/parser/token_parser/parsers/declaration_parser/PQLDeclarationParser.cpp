#include "PQLDeclarationParser.h"

void PQLDeclarationParser::parse(QueryTokenParseState *parserState,
                                 QueryBuilder *queryBuilder) {
  IPQLDeclarationContext* context = getContext(
      parserState->getCurrentTokenType());
  while (context != nullptr) {
    parserState->advanceToken();
    context->parse(parserState, queryBuilder);
    context = getContext(parserState->getCurrentTokenType());
  }
  queryBuilder->finalizeSynonymTable();
}

IPQLDeclarationContext *PQLDeclarationParser::getContext(PQLTokenType type) {
  switch (type) {
    case PQL_TOKEN_STMT:
      return &stmtContext;
    case PQL_TOKEN_READ:
      return &readContext;
    case PQL_TOKEN_PRINT:
      return &printContext;
    case PQL_TOKEN_CALL:
      return &callContext;
    case PQL_TOKEN_WHILE:
      return &whileContext;
    case PQL_TOKEN_IF:
      return &ifContext;
    case PQL_TOKEN_ASSIGN:
      return &assignContext;
    case PQL_TOKEN_VARIABLE:
      return &variableContext;
    case PQL_TOKEN_CONSTANT:
      return &constantContext;
    case PQL_TOKEN_PROCEDURE:
      return &procedureContext;
    default:
      return nullptr;
  }
}
