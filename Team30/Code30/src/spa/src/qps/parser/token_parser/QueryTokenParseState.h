#pragma once

#include <vector>
#include <unordered_set>
#include <string>
#include "../builder/QueryBuilder.h"
#include "token_stream/QueryTokenStream.h"
#include "../../errors/QPSParserSyntaxError.h"
#include "SemanticErrorStore.h"
#include "QueryExpectationAgent.h"

using std::vector, std::unordered_set, std::string;

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

class QueryTokenParseState:
    public SemanticErrorStore,
    public QueryExpectationAgent {
 private:
  QueryTokenStream tokenStream;
  QueryBuilder queryBuilder;
  TokenParsingStage currentStage;

 public:
  explicit QueryTokenParseState(vector<PQLToken>* tokens);
  bool isTokenStreamEnd();
  void advanceToken();
  PQLToken* getCurrentToken();

  void advanceStage(TokenParsingStage newStage);
  QueryBuilder* getQueryBuilder();
};
