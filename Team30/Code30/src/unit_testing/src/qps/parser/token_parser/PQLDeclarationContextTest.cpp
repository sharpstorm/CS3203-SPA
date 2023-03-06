#include "catch.hpp"

#include <memory>
#include <string>

#include "PQLContextTestUtils.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/parser/lexer/QueryLexerTokenTable.h"
#include "qps/parser/token_parser/parsers/declaration_parser/contexts/PQLDeclarationContextTypes.h"

using std::make_unique, std::string;

TEST_CASE("Test PQL Declaration parsing") {
  vector<string> vars = vector<string>{"s", "s0", "a0s", "a00sdf00", "abcdefd",
                                       "asDFG", "ASD", "A0Sw9"};

  for (string v : vars) {
    testDeclarationParsing<PQLStmtContext>(
        make_unique<PQLTestTokenSequenceBuilder>()
            ->synonym(v)
            ->semicolon()
            ->build(), v);
  }
}

TEST_CASE("Test PQL Declaration Keyword Name") {
  QueryLexerTokenTable tokenTable;
  for (auto it : tokenTable.keywordMap) {
    testDeclarationParsing<PQLStmtContext>(
        make_unique<PQLTestTokenSequenceBuilder>()
            ->addToken(it.second, it.first)
            ->semicolon()
            ->build(), it.first);
  }
}

TEST_CASE("Test PQL Declaration No semicolon") {
  REQUIRE_THROWS_AS(testDeclarationParsing<PQLStmtContext>(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->synonym("s")
          ->build(), "s"), QPSParserSyntaxError);
}
