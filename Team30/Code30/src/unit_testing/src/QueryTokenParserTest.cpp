#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "catch.hpp"
#include "qps/parser/lexer/QueryLexer.h"
#include "qps/parser/token_parser/TokenParser.h"
#include "qps/parser/PQLToken.h"
#include "qps/common/PQLQuery.h"
#include "qps/errors/QPSError.h"

using std::vector, std::string, std::tuple;

void testPQLParsing(vector<PQLToken> testcase, vector<QueryVariable> expectedVariables) {
  TokenParser parser(testcase);
  try {
    auto result = parser.build();
    REQUIRE(result->getVariableCount() == expectedVariables.size());
    for (int i = 0; i < expectedVariables.size(); i++) {
      auto expectedVar = expectedVariables.at(i);
      auto var = result->getVariable(expectedVar.name);
      REQUIRE(var != nullptr);
      REQUIRE(var->name == expectedVar.name);
      REQUIRE(var->type == expectedVar.type);
    }
  } catch (const QPSError& err) {
    FAIL("FAILED with error: " + err.message);
  }


}

TEST_CASE("Test QPS Parser Variables") {
  tuple<string, PQL_TOKEN_TYPE, PQL_VAR_TYPE> types[] = {
      { "stmt", PQL_TOKEN_STMT, PQL_VAR_TYPE_STMT },
  };

  for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
    testPQLParsing(vector<PQLToken>{
        PQLToken{std::get<1>(types[i]), std::get<0>(types[i])},
        PQLToken{PQL_TOKEN_STRING, "a"},
        PQLToken{PQL_TOKEN_SEMICOLON, ";"},
    }, vector<QueryVariable>{
        QueryVariable{std::get<2>(types[i]), "a"}
    });

    testPQLParsing(vector<PQLToken>{
        PQLToken{std::get<1>(types[i]), std::get<0>(types[i])},
        PQLToken{PQL_TOKEN_STRING, "a"},
        PQLToken{PQL_TOKEN_COMMA, ","},
        PQLToken{PQL_TOKEN_STRING, "b"},
        PQLToken{PQL_TOKEN_SEMICOLON, ";"},
    }, vector<QueryVariable>{
        QueryVariable{std::get<2>(types[i]), "a"},
        QueryVariable{std::get<2>(types[i]), "b"}
    });
  }
}
