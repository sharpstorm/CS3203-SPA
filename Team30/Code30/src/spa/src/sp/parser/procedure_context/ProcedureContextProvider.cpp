#include "ProcedureContextProvider.h"

ProcedureContextProvider::ProcedureContextProvider(
    IEntityParser *entityParser,
    IExpressionParser *exprParser,
    IConditionalParser *condParser) :
    entityParser(entityParser), exprParser(exprParser), condParser(condParser),
    procedureContext(this),
    stmtListContext(this),
    stmtContext(this),
    assignContext(this),
    callContext(this),
    ifContext(this),
    printContext(this),
    readContext(this),
    whileContext(this) {}

ASTNodePtr ProcedureContextProvider::generateSubtree(ProcedureContextType type,
                                                     SourceParseState *state)
const {
  return getContext(type)->generateSubtree(state);
}

ASTNodePtr ProcedureContextProvider::parseVariable(SourceParseState *state)
const {
  return entityParser->parseVariable(state);
}

ASTNodePtr ProcedureContextProvider::parseCondition(SourceParseState *state)
const {
  return condParser->parse(state);
}

ASTNodePtr ProcedureContextProvider::parseExpression(SourceParseState *state)
const {
  return exprParser->parse(state);
}

const SourceParseContext *ProcedureContextProvider::getContext(
    ProcedureContextType type) const {
  switch (type) {
    case ProcedureContextType::PROCEDURE_CONTEXT:return &procedureContext;
    case ProcedureContextType::STMT_LIST_CONTEXT:return &stmtListContext;
    case ProcedureContextType::STMT_CONTEXT:return &stmtContext;

    case ProcedureContextType::ASSIGN_CONTEXT:return &assignContext;
    case ProcedureContextType::CALL_CONTEXT:return &callContext;
    case ProcedureContextType::IF_CONTEXT:return &ifContext;
    case ProcedureContextType::PRINT_CONTEXT:return &printContext;
    case ProcedureContextType::READ_CONTEXT:return &readContext;
    case ProcedureContextType::WHILE_CONTEXT:return &whileContext;
    default:throw SPError(SPERR_CONTEXT_ERROR);
  }
}
