#include "catch.hpp"

#include <unordered_map>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLFollowsClauseContext.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::unordered_map;

template <class ParseContext, class ClauseType>
void testSuchThatParsing(vector<PQLToken> inputs,
                         unordered_map<string, PQLSynonymType> synonyms) {
  ParseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);

  for (auto it : synonyms) {
    state.getQueryBuilder()->addVariable(it.first, it.second);
  }

  context.parse(&state);
  state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END);
  state.throwIfSemanticError();

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<ClauseType*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}
