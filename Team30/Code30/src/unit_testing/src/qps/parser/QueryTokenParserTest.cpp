#include <string>
#include <vector>
#include <tuple>

#include "catch.hpp"
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
  tuple<string, PQLTokenType, PQLSynonymType> types[] = {
      { "stmt", PQL_TOKEN_STMT, PQL_VAR_TYPE_STMT },
      { "read", PQL_TOKEN_READ, PQL_VAR_TYPE_READ },
      { "print", PQL_TOKEN_PRINT, PQL_VAR_TYPE_PRINT },
      { "call", PQL_TOKEN_CALL, PQL_VAR_TYPE_CALL },
      { "while", PQL_TOKEN_WHILE, PQL_VAR_TYPE_WHILE },
      { "if", PQL_TOKEN_IF, PQL_VAR_TYPE_IF },
      { "assign", PQL_TOKEN_ASSIGN, PQL_VAR_TYPE_ASSIGN },
      { "variable", PQL_TOKEN_VARIABLE, PQL_VAR_TYPE_VARIABLE },
      { "constant", PQL_TOKEN_CONSTANT, PQL_VAR_TYPE_CONSTANT },
      { "procedure", PQL_TOKEN_PROCEDURE, PQL_VAR_TYPE_PROCEDURE },
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
