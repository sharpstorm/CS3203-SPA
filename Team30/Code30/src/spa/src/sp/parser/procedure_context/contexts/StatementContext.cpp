#include "StatementContext.h"

ASTNodePtr StatementContext::generateSubtree(
    SourceParseState *state) {

  if (state->nextTokenIsOfType(SIMPLE_TOKEN_ASSIGN)) {
    return invokeSubcontext<ProcedureContextType::ASSIGN_CONTEXT>(state);
  }

  SourceToken* currentToken = state->getCurrToken();
  if (currentToken == nullptr) {
    throw SPError("Unknown token sequence");
  }

  switch (currentToken->getType()) {
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
      throw SPError("Unknown token sequence");
  }
}

template<ProcedureContextType CONTEXT_TYPE>
ASTNodePtr StatementContext::invokeSubcontext(SourceParseState *state) {
  return contextProvider
      ->generateSubtree(CONTEXT_TYPE, state);
}
