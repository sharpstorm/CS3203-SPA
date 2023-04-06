#include "StatementContext.h"

ASTNodePtr StatementContext::generateSubtree(SourceParseState *state) const {
  state->advanceLine();
  if (state->nextTokenIsOfType(SIMPLE_TOKEN_ASSIGN)) {
    return invokeSubcontext<ProcedureContextType::ASSIGN_CONTEXT>(state);
  }

  if (state->isEnd()) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  switch (state->getCurrTokenType()) {
    case SIMPLE_TOKEN_KEYWORD_PRINT:
      return invokeSubcontext<ProcedureContextType::PRINT_CONTEXT>(state);
    case SIMPLE_TOKEN_KEYWORD_IF:
      return invokeSubcontext<ProcedureContextType::IF_CONTEXT>(state);
    case SIMPLE_TOKEN_KEYWORD_WHILE:
      return invokeSubcontext<ProcedureContextType::WHILE_CONTEXT>(state);
    case SIMPLE_TOKEN_KEYWORD_READ:
      return invokeSubcontext<ProcedureContextType::READ_CONTEXT>(state);
    case SIMPLE_TOKEN_KEYWORD_CALL:
      return invokeSubcontext<ProcedureContextType::CALL_CONTEXT>(state);
    default:
      throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
}

template<ProcedureContextType CONTEXT_TYPE>
ASTNodePtr StatementContext::invokeSubcontext(SourceParseState *state) const {
  return contextProvider->generateSubtree(CONTEXT_TYPE, state);
}
