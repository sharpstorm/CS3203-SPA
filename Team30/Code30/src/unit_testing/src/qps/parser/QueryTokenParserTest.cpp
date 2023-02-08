#include <string>
#include <vector>
#include <tuple>

#include "catch.hpp"
#include "qps/parser/token_parser/QueryTokenParser.h"
#include "qps/parser/PQLToken.h"
#include "qps/common/PQLQuery.h"
#include "qps/common/PQLQueryVariable.h"
#include "qps/errors/QPSError.h"
#include "qps/clauses/FollowsClause.h"
#include "qps/clauses/ParentClause.h"

using std::vector, std::string, std::tuple, std::exception;

tuple<string, PQLTokenType, PQLSynonymType> TEST_TYPE_MAP[] = {
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

unique_ptr<PQLQuery> testPQLParsing(vector<PQLToken> testcase, vector<PQLQueryVariable> expectedVariables) {
  QueryTokenParser parser(testcase);
  unique_ptr<PQLQuery> result;
  try {
    result = parser.build();
  } catch (const QPSError& err) {
    FAIL("FAILED with error: " + err.message);
  } catch (...) {
    FAIL("Failed with unknown exception");
  }

  REQUIRE(result->getVariableCount() == expectedVariables.size());
  for (int i = 0; i < expectedVariables.size(); i++) {
    auto expectedVar = expectedVariables.at(i);
    auto var = result->getVariable(expectedVar.name);
    REQUIRE(var != nullptr);
    REQUIRE(var->name == expectedVar.name);
    REQUIRE(var->type == expectedVar.type);
  }

  return result;
}

void testPQLParsingRejection(vector<PQLToken> testcase) {
  QueryTokenParser parser(testcase);
  try {
    auto result = parser.build();
    FAIL("Test did not fail with error");
  } catch (const QPSError& err) {
  } catch (...) {
    FAIL("Test failed with unknown error");
  }
}

TEST_CASE("Test QPS Parser Variables") {
  for (int i = 0; i < sizeof(TEST_TYPE_MAP) / sizeof(TEST_TYPE_MAP[0]); i++) {
    testPQLParsing(vector<PQLToken>{
        PQLToken{std::get<1>(TEST_TYPE_MAP[i]), std::get<0>(TEST_TYPE_MAP[i])},
        PQLToken{PQL_TOKEN_STRING, "a"},
        PQLToken{PQL_TOKEN_SEMICOLON, ";"},
        PQLToken{PQL_TOKEN_SELECT, ""},
        PQLToken{PQL_TOKEN_STRING, "a"},
    }, vector<PQLQueryVariable>{
        PQLQueryVariable{std::get<2>(TEST_TYPE_MAP[i]), "a"}
    });

    testPQLParsing(vector<PQLToken>{
        PQLToken{std::get<1>(TEST_TYPE_MAP[i]), std::get<0>(TEST_TYPE_MAP[i])},
        PQLToken{PQL_TOKEN_STRING, "a"},
        PQLToken{PQL_TOKEN_COMMA, ","},
        PQLToken{PQL_TOKEN_STRING, "b"},
        PQLToken{PQL_TOKEN_SEMICOLON, ";"},
        PQLToken{PQL_TOKEN_SELECT, ""},
        PQLToken{PQL_TOKEN_STRING, "a"},
    }, vector<PQLQueryVariable>{
        PQLQueryVariable{std::get<2>(TEST_TYPE_MAP[i]), "a"},
        PQLQueryVariable{std::get<2>(TEST_TYPE_MAP[i]), "b"}
    });
  }
}

TEST_CASE("Test QPS Parser Keyword Variable Name") {
  for (int i = 0; i < sizeof(TEST_TYPE_MAP) / sizeof(TEST_TYPE_MAP[0]); i++) {
    testPQLParsing(vector<PQLToken>{
        PQLToken{std::get<1>(TEST_TYPE_MAP[i]), std::get<0>(TEST_TYPE_MAP[i])},
        PQLToken{std::get<1>(TEST_TYPE_MAP[i]), std::get<0>(TEST_TYPE_MAP[i])},
        PQLToken{PQL_TOKEN_SEMICOLON, ";"},
        PQLToken{PQL_TOKEN_SELECT, ""},
        PQLToken{PQL_TOKEN_STRING, std::get<0>(TEST_TYPE_MAP[i])},
    }, vector<PQLQueryVariable>{
        PQLQueryVariable{std::get<2>(TEST_TYPE_MAP[i]), std::get<0>(TEST_TYPE_MAP[i])}
    });
  }
}

TEST_CASE("Test QPS Parser Duplicate Variable Rejection") {
  for (int i = 0; i < sizeof(TEST_TYPE_MAP) / sizeof(TEST_TYPE_MAP[0]); i++) {
    testPQLParsingRejection(vector<PQLToken>{
        PQLToken{std::get<1>(TEST_TYPE_MAP[i]), std::get<0>(TEST_TYPE_MAP[i])},
        PQLToken{PQL_TOKEN_STRING, "a"},
        PQLToken{PQL_TOKEN_COMMA, ","},
        PQLToken{PQL_TOKEN_STRING, "a"},
        PQLToken{PQL_TOKEN_SEMICOLON, ";"},
    });
  }
}

TEST_CASE("Test QPS Parser Follows Query") {
  auto query = testPQLParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT, "stmt"},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_SEMICOLON, ";"},
      PQLToken{PQL_TOKEN_SELECT},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_SUCH},
      PQLToken{PQL_TOKEN_THAT},
      PQLToken{PQL_TOKEN_FOLLOWS},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},

  }, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "a"}
  });
  REQUIRE(query->getEvaluatables().size() == 1);

  FollowsClause* fc = dynamic_cast<FollowsClause*>(query->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);

  query = testPQLParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT, "stmt"},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "b"},
      PQLToken{PQL_TOKEN_SEMICOLON, ";"},
      PQLToken{PQL_TOKEN_SELECT},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_SUCH},
      PQLToken{PQL_TOKEN_THAT},
      PQLToken{PQL_TOKEN_FOLLOWS},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "b"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},

  }, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "a"},
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "b"}
  });
  REQUIRE(query->getEvaluatables().size() == 1);

  fc = dynamic_cast<FollowsClause*>(query->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test QPS Parser Parent Query") {
  auto query = testPQLParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT, "stmt"},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_SEMICOLON, ";"},
      PQLToken{PQL_TOKEN_SELECT},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_SUCH},
      PQLToken{PQL_TOKEN_THAT},
      PQLToken{PQL_TOKEN_PARENT},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_INTEGER, "2"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_INTEGER, "1"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},

  }, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "a"}
  });
  REQUIRE(query->getEvaluatables().size() == 1);

  ParentClause* fc = dynamic_cast<ParentClause*>(query->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);

  query = testPQLParsing(vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT, "stmt"},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "b"},
      PQLToken{PQL_TOKEN_SEMICOLON, ";"},
      PQLToken{PQL_TOKEN_SELECT},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_SUCH},
      PQLToken{PQL_TOKEN_THAT},
      PQLToken{PQL_TOKEN_PARENT},
      PQLToken{PQL_TOKEN_BRACKET_OPEN},
      PQLToken{PQL_TOKEN_STRING, "a"},
      PQLToken{PQL_TOKEN_COMMA},
      PQLToken{PQL_TOKEN_STRING, "b"},
      PQLToken{PQL_TOKEN_BRACKET_CLOSE},

  }, vector<PQLQueryVariable>{
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "a"},
      PQLQueryVariable{PQL_VAR_TYPE_STMT, "b"}
  });
  REQUIRE(query->getEvaluatables().size() == 1);

  fc = dynamic_cast<ParentClause*>(query->getEvaluatables().at(0).get());
  REQUIRE(fc != nullptr);
}