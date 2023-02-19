#include "StatementContext.h"

ASTNodePtr StatementContext::generateSubtree(
    SourceParseState *state) {
  state->advanceLine();
  state->clearCached();

  if (state->nextTokenIsOfType(SIMPLE_TOKEN_ASSIGN)) {
    return contextProvider
        ->getContext(ASSIGN_CONTEXT)->generateSubtree(state);
  }

  SourceGrammarContextType context;
  SourceToken* currentToken = state->getCurrToken();
  if (currentToken == nullptr) {
    throw SPError("Unknown token sequence");
  }

  switch (currentToken->getType()) {
    case SIMPLE_TOKEN_KEYWORD_PRINT:
      return contextProvider
      ->getContext(PRINT_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_IF:
      return contextProvider
      ->getContext(IF_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_WHILE:
      return contextProvider
          ->getContext(WHILE_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_READ:
      return contextProvider
          ->getContext(READ_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_CALL:
      return contextProvider
          ->getContext(CALL_CONTEXT)->generateSubtree(state);
    default:
      throw SPError("Unknown token sequence");
  }
}
