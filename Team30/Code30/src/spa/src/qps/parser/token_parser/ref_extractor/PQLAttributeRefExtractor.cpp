#include <memory>

#include "PQLAttributeRefExtractor.h"

using std::make_unique;

AttributedSynonymPtr PQLAttributeRefExtractor::extract(
    QueryTokenParseState* state, QueryBuilder* builder) {
  PQLSynonymName synName = state->expectSynName();
  PQLSynonymAttribute attr = NO_ATTRIBUTE;

  bool hasAttr = state->tryExpect(PQL_TOKEN_PERIOD) != nullptr;
  if (hasAttr) {
    attr = extractAttribute(state);
  }

  PQLQuerySynonymProxy* var = builder->accessSynonym(synName);
  if (var == nullptr) {
    return nullptr;
  }

  return make_unique<AttributedSynonym>(*var, attr);
}

PQLSynonymAttribute PQLAttributeRefExtractor::extractAttribute(
    QueryTokenParseState *state) {
  PQLToken* token = state->tryExpect(
      PQL_TOKEN_VALUE,
      PQL_TOKEN_STMT,
      PQL_TOKEN_PROC_NAME,
      PQL_TOKEN_VAR_NAME);

  if (token == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  switch (token->getType()) {
    case PQL_TOKEN_VAR_NAME:
      return VAR_NAME;
    case PQL_TOKEN_PROC_NAME:
      return PROC_NAME;
    case PQL_TOKEN_STMT:
      state->expect(PQL_TOKEN_NUMBER_SIGN);
      return STMT_NUM;
    case PQL_TOKEN_VALUE:
      return CONST_VALUE;
    default:
      throw QPSParserSyntaxError(QPS_PARSER_ERR_WITH_TYPE);
  }
}
