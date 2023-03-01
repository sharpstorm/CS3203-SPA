#include "GrammarContextProvider.h"

GrammarContextProvider::GrammarContextProvider() :
    factorContext(this),
    termContext(this), expressionContext(this),
    relationalFactorContext(this),
    relationalExpressionContext(this),
    conditionalExpressionContext(this),
    assignContext(this), printContext(this),
    whileContext(this), statementContext(this),
    ifContext(this),
    readContext(this),
    procedureContext(this),
    statementListContext(this),
    callContext(this),
    programContext(this) {}

SourceParseContext* GrammarContextProvider::getContext(
    SourceGrammarContextType type) {
  switch (type) {
    case PROGRAM_CONTEXT:
      return &programContext;
    case PROCEDURE_CONTEXT:
      return &procedureContext;
    case CALL_CONTEXT:
      return &callContext;
    case STMT_LIST_CONTEXT:
      return &statementListContext;
    case STMT_CONTEXT:
      return &statementContext;
    case IF_CONTEXT:
      return &ifContext;
    case WHILE_CONTEXT:
      return &whileContext;
    case PRINT_CONTEXT:
      return &printContext;
    case ASSIGN_CONTEXT:
      return &assignContext;
    case COND_CONTEXT:
      return &conditionalExpressionContext;
    case REL_CONTEXT:
      return &relationalExpressionContext;
    case REL_FACTOR_CONTEXT:
      return &relationalFactorContext;
    case EXPR_CONTEXT:
      return &expressionContext;
    case TERM_CONTEXT:
      return &termContext;
    case FACTOR_CONTEXT:
      return &factorContext;
    case CONST_CONTEXT:
      return &constantParseContext;
    case VARIABLE_CONTEXT:
      return &variableParseContext;
    case READ_CONTEXT:
      return &readContext;
  }
  return nullptr;
}
