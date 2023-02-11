#include <memory>
#include "StatementContext.h"

using std::shared_ptr;

shared_ptr<ASTNode> StatementContext::generateSubtree(
    SourceParseState *state) {;
  state->advanceLine();
  state->clearCached();
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_KEYWORD_PRINT:
      return contextProvider->
          getContext(PRINT_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_IF:
      return contextProvider->
          getContext(IF_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_WHILE:
      return contextProvider->
          getContext(WHILE_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_READ:
      return contextProvider->
          getContext(READ_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_VARIABLE:
      return contextProvider->
          getContext(ASSIGN_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_KEYWORD_CALL:
      throw SPError("Not implemented");
    default:
      throw SPError("Unknown token sequence");
  }
}
