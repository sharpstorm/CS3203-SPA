#include <memory>
#include <utility>

#include "PQLAttributeRefExtractor.h"

using std::make_unique;

WithArgumentPtr PQLAttributeRefExtractor::extract(QueryTokenParseState* state,
                                                    QueryBuilder* builder) {
  PQLToken* token = state->tryExpect(
      PQL_TOKEN_STRING_LITERAL, PQL_TOKEN_INTEGER);

  if (token != nullptr) {
    if (token->isType(PQL_TOKEN_STRING_LITERAL)) {
      return make_unique<WithArgument>(token->getData());
    } else {
      int intVal = stoi(token->getData());
      return make_unique<WithArgument>(intVal);
    }
  }

  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonym* var = builder->accessSynonym(synName);

  state->expect(PQL_TOKEN_PERIOD);
  PQLSynonymAttribute attr = extractAttribute(state);

  if (var == nullptr) {
    return nullptr;
  }

  AttributedSynonymPtr attrSynPtr = make_unique<AttributedSynonym>(var, attr);

  return make_unique<WithArgument>(std::move(attrSynPtr));
}


PQLSynonymAttribute PQLAttributeRefExtractor::extractAttribute(
    QueryTokenParseState *parserState) {
  PQLToken* token = parserState->tryExpect(
      PQL_TOKEN_VALUE, PQL_TOKEN_STMT, PQL_TOKEN_PROC_NAME, PQL_TOKEN_VAR_NAME);

  if (token == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  switch (token->getType()) {
    case PQL_TOKEN_VAR_NAME:return VAR_NAME;
    case PQL_TOKEN_PROC_NAME:return PROC_NAME;
    case PQL_TOKEN_STMT:
      parserState->expect(PQL_TOKEN_NUMBER_SIGN);
      return STMT_NUM;
    case PQL_TOKEN_VALUE:return CONST_VALUE;
    default:
      throw QPSParserSyntaxError(QPS_PARSER_ERR_WITH_TYPE);
  }
}
