#include "catch.hpp"

#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLFollowsClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/FollowsClause.h"

using std::make_unique;

void testFollowsParsing(vector<PQLToken> inputs) {
  PQLFollowsClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<FollowsClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Follows parsing 2 Constants") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(1)
                         ->comma()
                         ->integer(2)
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows 1 Constant Left") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->synonym("s")
                         ->comma()
                         ->integer(2)
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->wildcard()
                         ->comma()
                         ->integer(2)
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows 1 Constant Right") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(2)
                         ->comma()
                         ->synonym("s")
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(2)
                         ->comma()
                         ->wildcard()
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows 0 Constant") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->synonym("s")
                         ->comma()
                         ->synonym("s")
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->synonym("s")
                         ->comma()
                         ->wildcard()
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->wildcard()
                         ->comma()
                         ->synonym("s")
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->wildcard()
                         ->comma()
                         ->wildcard()
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows unknown ref") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->synonym("w")
                             ->comma()
                             ->integer(2)
                             ->closeBracket()
                             ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Follows Entity ref not allowed") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->ident("w")
                             ->comma()
                             ->integer(2)
                             ->closeBracket()
                             ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->comma()
                             ->ident("w")
                             ->closeBracket()
                             ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Follows bad syntax") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->synonym("w")
                             ->closeBracket()
                             ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->comma()
                             ->synonym("w")
                             ->build()
      ), QPSParserError
  );
}
