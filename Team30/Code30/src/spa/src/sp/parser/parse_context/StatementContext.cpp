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
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_KEYWORD_PRINT:
      context = PRINT_CONTEXT;
      break;
    case SIMPLE_TOKEN_KEYWORD_IF:
      context = IF_CONTEXT;
      break;
    case SIMPLE_TOKEN_KEYWORD_WHILE:
      context = WHILE_CONTEXT;
      break;
    case SIMPLE_TOKEN_KEYWORD_READ:
      context = READ_CONTEXT;
      break;
    case SIMPLE_TOKEN_KEYWORD_CALL:
      context = CALL_CONTEXT;
      break;
    default:
      throw SPError("Unknown token sequence");
  }

  return contextProvider->getContext(context)->generateSubtree(state);
}
