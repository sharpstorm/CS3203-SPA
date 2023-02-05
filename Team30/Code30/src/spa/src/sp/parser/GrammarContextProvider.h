#pragma once

#include "../common/SourceToken.h"
#include "SourceParseState.h"
#include "IGrammarContextProvider.h"
#include "parse_context/SourceParseContext.h"
#include "parse_context/VariableParseContext.h"
#include "parse_context/ConstantParseContext.h"
#include "parse_context/FactorContext.h"
#include "parse_context/TermContext.h"
#include "parse_context/ExpressionContext.h"
#include "parse_context/RelationalFactorContext.h"
#include "parse_context/RelationalExpressionContext.h"
#include "sp/parser/parse_context/ConditionalExpressionContext.h"
#include "sp/parser/parse_context/StatementContext/AssignContext.h"
#include "sp/parser/parse_context/StatementContext/PrintContext.h"
#include "sp/parser/parse_context/StatementContext/WhileContext.h"
#include "sp/parser/parse_context/StatementContext.h"
#include "sp/parser/parse_context/StatementContext/IfContext.h"
#include "sp/parser/parse_context/StatementContext/ReadContext.h"
#include "sp/parser/parse_context/ProcedureContext.h"
#include "sp/parser/parse_context/StatementListContext.h"

class GrammarContextProvider: public IGrammarContextProvider {
 public:
  GrammarContextProvider();
  SourceParseContext* getContext(SourceGrammarContextType type);
  int currLineCounter();
  void advanceLineCounter();
 private:
  int lineCounter = 1;
  VariableParseContext variableParseContext;
  ConstantParseContext constantParseContext;
  FactorContext factorContext;
  TermContext termContext;
  ExpressionContext expressionContext;
  RelationalFactorContext relationalFactorContext;
  RelationalExpressionContext relationalExpressionContext;
  ConditionalExpressionContext conditionalExpressionContext;
  AssignContext assignContext;
  PrintContext printContext;
  WhileContext whileContext;
  StatementContext statementContext;
  IfContext ifContext;
  ReadContext readContext;
  ProcedureContext procedureContext;
  StatementListContext statementListContext;
};
