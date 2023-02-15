#pragma once

#include <vector>
#include <unordered_set>
#include "../PQLToken.h"
#include "../builder/QueryBuilder.h"
#include "token_stream/QueryTokenStream.h"
#include "PQLParserErrors.h"
#include "../../errors/QPSParserError.h"

using std::vector, std::unordered_set;

enum TokenParsingStage {
  TOKEN_PARSE_STAGE_INIT,
  TOKEN_PARSE_STAGE_DECLARATION,
  TOKEN_PARSE_STAGE_COMMAND,
  TOKEN_PARSE_STAGE_CONDITION_MARKER,
  TOKEN_PARSE_STAGE_CONDITION,
  TOKEN_PARSE_STAGE_PATTERN_MARKER,
  TOKEN_PARSE_STAGE_PATTERN,
  TOKEN_PARSE_STAGE_PARSE_END,
};

// Follows the ENUM ordering above
const unordered_set<TokenParsingStage> parsingAllowedTransitions[] = {
    { TOKEN_PARSE_STAGE_DECLARATION, TOKEN_PARSE_STAGE_COMMAND },
    { TOKEN_PARSE_STAGE_DECLARATION, TOKEN_PARSE_STAGE_COMMAND },
    { TOKEN_PARSE_STAGE_CONDITION_MARKER,
      TOKEN_PARSE_STAGE_PATTERN_MARKER, TOKEN_PARSE_STAGE_PARSE_END },
    { TOKEN_PARSE_STAGE_CONDITION },
    { TOKEN_PARSE_STAGE_CONDITION_MARKER,
      TOKEN_PARSE_STAGE_PATTERN_MARKER, TOKEN_PARSE_STAGE_PARSE_END },
    { TOKEN_PARSE_STAGE_PATTERN },
    { TOKEN_PARSE_STAGE_CONDITION_MARKER,
      TOKEN_PARSE_STAGE_PATTERN_MARKER, TOKEN_PARSE_STAGE_PARSE_END },
    {},
};

class QueryTokenParseState {
 private:
  QueryTokenStream tokenStream;
  QueryBuilder queryBuilder;
  TokenParsingStage currentStage;

 public:
  explicit QueryTokenParseState(vector<PQLToken>* tokens);
  bool isTokenStreamEnd();
  void advanceToken();
  PQLToken* getCurrentToken();

  template<typename... T>
  PQLToken *expect(T... tokenType);
  template<typename... T>
  PQLToken *expectCategory(T... tokenCategory);
  PQLToken *expectVarchar();

  void advanceStage(TokenParsingStage newStage);
  QueryBuilder* getQueryBuilder();
};


template<typename... PQLTokenType>
PQLToken* QueryTokenParseState::expect(PQLTokenType... tokenType) {
  PQLToken* currentToken = getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_EOS);
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    advanceToken();
    return currentToken;
  }

  throw QPSParserError(QPS_PARSER_ERR_UNEXPECTED);
}

template<typename... PQLTokenCategory>
PQLToken* QueryTokenParseState::expectCategory(PQLTokenCategory... tokenCategory) {
  PQLToken* currentToken = getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_EOS);
  }

  if ((currentToken->isCategory(tokenCategory)  || ... || false)) {
    advanceToken();
    return currentToken;
  }

  throw QPSParserError(QPS_PARSER_ERR_UNEXPECTED);
}
