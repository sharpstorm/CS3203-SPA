#include "GrammarContextProvider.h"

using namespace std;



GrammarContextProvider::GrammarContextProvider() : factorContext(this),
termContext(this), expressionContext(this), relationalFactorContext(this),
                                                   relationalExpressionContext(this){

}

SourceParseContext* GrammarContextProvider::getContext(
    SourceGrammarContextType type) {
  switch (type) {
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
  }

  return nullptr;
}
