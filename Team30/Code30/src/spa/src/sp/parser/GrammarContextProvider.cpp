#include "GrammarContextProvider.h"

using namespace std;



GrammarContextProvider::GrammarContextProvider() : factorContext(this),
termContext(this), expressionContext(this) {

}

SourceParseContext* GrammarContextProvider::getContext(
    SourceGrammarContextType type) {
  switch (type) {
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
  /*
  switch (state->getCurrTokenType()) {
      case (Token::SIMPLE_TYPE_NAME):
          if (state->getCurrTokenValue() == "procedure") {

          } else if (state->getCurrTokenValue() == "read") {

          } else if (state->getCurrTokenValue() == "print") {

          } else if (state->getCurrTokenValue() == "while") {

          } else if (state->getCurrTokenValue() == "if") {

          } else {
              ctx = &variableParseContext;
          }
          break;
      case (Token::SIMPLE_TYPE_TIMES):
      case (Token::SIMPLE_TYPE_DIV):
      case (Token::SIMPLE_TYPE_MOD):
          ctx = &termParseContext;
          break;
      case (Token::SIMPLE_TYPE_MINUS):
      case (Token::SIMPLE_TYPE_PLUS):
          ctx = &expressionParseContext;
          break;
      case (Token::SIMPLE_TYPE_INTEGER):
          ctx =  &constantParseContext;
          break;
      case (Token::SIMPLE_TYPE_BRACKET_ROUND_LEFT):
          ctx = &factorParseContext;
          break;
  }*/
  return nullptr;
}
