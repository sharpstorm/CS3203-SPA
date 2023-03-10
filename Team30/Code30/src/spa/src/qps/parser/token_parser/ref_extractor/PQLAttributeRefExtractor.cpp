#include <memory>

#include "PQLAttributeRefExtractor.h"
#include "qps/clauses/arguments/WithArgumentFactory.h"

using std::make_unique;

WithArgumentPtr PQLAttributeRefExtractor::extract(QueryTokenParseState* state,
                                                    QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_STRING_LITERAL)) {
    PQLToken* entRef = state->expect(PQL_TOKEN_STRING_LITERAL);
    return WithArgumentFactory::create(entRef->getData());
  } else if (state->isCurrentTokenType(PQL_TOKEN_INTEGER)) {
    PQLToken* stmtRef = state->expect(PQL_TOKEN_INTEGER);
    int intVal = stoi(stmtRef->getData());
    return WithArgumentFactory::create(intVal);
  }

  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonym* var = builder->accessSynonym(synName);
  if (var == nullptr) {
    return nullptr;
  }

  state->expect(PQL_TOKEN_PERIOD);
  PQLSynonymAttribute attr = extractAttribute(state, var->getType());
  AttributedSynonym attrSyn(*var, attr);

  return WithArgumentFactory::create(attrSyn);
}

PQLSynonymAttribute PQLAttributeRefExtractor::extractAttribute(
    QueryTokenParseState *parserState, PQLSynonymType type) {
  PQLToken *foundToken = parserState->expectSynAttr();
  // TO HW: Test and check if it can take in the syn attributes.
  // Test if its the correct type via validateArgTypes I guess
  switch (foundToken->getType()) {
    case PQL_TOKEN_VAR_NAME:return PQLSynonymAttribute::VAR_NAME;
    case PQL_TOKEN_PROC_NAME:return PQLSynonymAttribute::PROC_NAME;
    case PQL_TOKEN_STMT:
      parserState->expect(PQL_TOKEN_NUMBER_SIGN);
      return PQLSynonymAttribute::STMT_NUM;
    case PQL_TOKEN_VALUE:return PQLSynonymAttribute::VALUE;
    default:throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }
}
