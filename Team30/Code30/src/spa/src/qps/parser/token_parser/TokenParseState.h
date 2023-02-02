#pragma once

#include <vector>
#include <unordered_set>
#include "../PQLToken.h"
#include "../builder/QueryBuilder.h"

using std::vector, std::unordered_set;

enum TokenParsingStage {
  TOKEN_PARSE_STAGE_INIT,
  TOKEN_PARSE_STAGE_DECLARATION,
  TOKEN_PARSE_STAGE_COMMAND,
  TOKEN_PARSE_STAGE_CONDITION,
  TOKEN_PARSE_STAGE_PATTERN,
  TOKEN_PARSE_STAGE_PARSE_END,
};

// Follows the ENUM ordering above
const unordered_set<TokenParsingStage> parsingAllowedTransitions[] = {
    { TOKEN_PARSE_STAGE_DECLARATION },
    { TOKEN_PARSE_STAGE_COMMAND },
    { TOKEN_PARSE_STAGE_CONDITION,
          TOKEN_PARSE_STAGE_PATTERN, TOKEN_PARSE_STAGE_PARSE_END },
    { TOKEN_PARSE_STAGE_PATTERN, TOKEN_PARSE_STAGE_PARSE_END },
    { TOKEN_PARSE_STAGE_PARSE_END },
    {},
};

class TokenParseState {
 private:
  int currentIndex;
  int totalTokenSize;
  vector<PQLToken>* tokens;
  QueryBuilder queryBuilder;
  TokenParsingStage currentStage;

 public:
  explicit TokenParseState(vector<PQLToken>* tokens);
  bool isTokenStreamEnd();
  void advanceToken();
  void advanceStage(TokenParsingStage newStage);
  QueryBuilder* getQueryBuilder();
  PQLToken* getCurrentToken();
};
