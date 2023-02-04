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

using namespace std;

class GrammarContextProvider: public IGrammarContextProvider {
 public:
  GrammarContextProvider();
  SourceParseContext* getContext(SourceGrammarContextType type);
 private:
  VariableParseContext variableParseContext;
  ConstantParseContext constantParseContext;
  FactorContext factorContext;
  TermContext termContext;
  ExpressionContext expressionContext;
  RelationalFactorContext relationalFactorContext;
  RelationalExpressionContext relationalExpressionContext;
};