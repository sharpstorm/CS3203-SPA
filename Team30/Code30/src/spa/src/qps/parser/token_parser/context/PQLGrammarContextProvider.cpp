#include "PQLGrammarContextProvider.h"

PQLGrammarContextProvider::PQLGrammarContextProvider(
    ISourceExpressionParser *exprParser): patternClauseContext(exprParser) {
}

IPQLContext* PQLGrammarContextProvider::getContext(PQLToken *token) {
  switch (token->getType()) {
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
    case PQL_TOKEN_SELECT:
      return &selectContext;
    case PQL_TOKEN_SUCH:
      return &suchThatContext;
    case PQL_TOKEN_FOLLOWS:
      return &followsClauseContext;
    case PQL_TOKEN_PARENT:
      return &parentClauseContext;
    case PQL_TOKEN_USES:
      return &usesClauseContext;
    case PQL_TOKEN_MODIFIES:
      return &modifiesClauseContext;
    case PQL_TOKEN_PATTERN:
      return &patternClauseContext;
    case PQL_TOKEN_CALLS:
      return &callsClauseContext;
    default:
      return nullptr;
  }
}
