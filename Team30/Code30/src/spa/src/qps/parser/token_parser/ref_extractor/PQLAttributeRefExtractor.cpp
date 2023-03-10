#include "PQLAttributeRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLAttributeRefExtractor::extract(QueryTokenParseState* state,
                                                    QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_STRING_LITERAL)) {
    return extractEntity(state);
  } else if (state->isCurrentTokenType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state, builder);
  }

  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonym* var = builder->accessSynonym(synName);
  if (var == nullptr) {
    return nullptr;
  }

  state->expect(PQL_TOKEN_PERIOD);
  PQLSynonymAttribute attr = extractAttribute(state, var->getType());
  var->setSynonymType(attr);

  return ClauseArgumentFactory::create(*var);
}

PQLSynonymAttribute PQLAttributeRefExtractor::extractAttribute(
    QueryTokenParseState *parserState, PQLSynonymType type) {
  PQLToken* foundToken = parserState->expectSynAttr();
  // TO HW: Test and check if it can take in the syn attributes.
  // Test if its the correct type via validateArgTypes I guess
  switch (foundToken->getType()) {
    case PQL_TOKEN_VAR_NAME:
      return PQLSynonymAttribute::VAR_NAME;
    case PQL_TOKEN_PROC_NAME:
      return PQLSynonymAttribute::PROC_NAME;
    case PQL_TOKEN_STMT_NUM:
      return PQLSynonymAttribute::STMT_NUM;
    case PQL_TOKEN_VALUE:
      return PQLSynonymAttribute::VALUE;
    default:
      throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }
//  switch (type) {
//    case PQL_SYN_TYPE_STMT:
//    case PQL_SYN_TYPE_ASSIGN:
//    case PQL_SYN_TYPE_IF:
//    case PQL_SYN_TYPE_WHILE:
//      foundToken = parserState->expect(PQL_TOKEN_STMT_NUM);
//      break;
//    case PQL_SYN_TYPE_READ:
//    case PQL_SYN_TYPE_PRINT:
//      foundToken = parserState->expect(PQL_TOKEN_STMT_NUM, PQL_TOKEN_VAR_NAME);
//      break;
//    case PQL_SYN_TYPE_CALL:
//      foundToken = parserState->expect(PQL_TOKEN_PROC_NAME, PQL_TOKEN_STMT_NUM);
//      break;
//    case PQL_SYN_TYPE_CONSTANT:
//      foundToken = parserState->expect(PQL_TOKEN_VALUE);
//      break;
//    case PQL_SYN_TYPE_VARIABLE:
//      foundToken = parserState->expect(PQL_TOKEN_VAR_NAME);
//      break;
//    case PQL_SYN_TYPE_PROCEDURE:
//      foundToken = parserState->expect(PQL_TOKEN_PROC_NAME);
//  }

  return getSynAttribute(foundToken);
}

PQLSynonymAttribute PQLAttributeRefExtractor::getSynAttribute(
    PQLToken * token) {
  switch (token->getType()) {
    case PQL_TOKEN_VAR_NAME:
      return PQLSynonymAttribute::VAR_NAME;
    case PQL_TOKEN_PROC_NAME:
      return PQLSynonymAttribute::PROC_NAME;
    case PQL_TOKEN_STMT_NUM:
      return PQLSynonymAttribute::STMT_NUM;
    case PQL_TOKEN_VALUE:
      return PQLSynonymAttribute::VALUE;
    default:
      throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }
}
