#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ASTNode/BinaryASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/GrammarContextProvider.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "../../../../../../spa/src/sp/parser/parse_context/math_context/ConditionalExpressionContext.h"

vector<SourceToken> notConditionInput() {  //(!(x == y) && !(x == z))
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_NOT, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
      SourceToken(SIMPLE_TOKEN_AND, ""),
      SourceToken(SIMPLE_TOKEN_NOT, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "z"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
  };

  return tokens;
}

vector<SourceToken> AndConditionInput() {  //((x != y) && (x != z))
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_NOT_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
      SourceToken(SIMPLE_TOKEN_AND, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_NOT_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "z"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
  };

  return tokens;
}

vector<SourceToken> OrConditionInput() {  //((x == y) || (x != z))
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
      SourceToken(SIMPLE_TOKEN_OR, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_NOT_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "z"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
  };

  return tokens;
}

vector<SourceToken> RelationalExpressionInput() {  //((x >= y) || (x < z))
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_GT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
      SourceToken(SIMPLE_TOKEN_OR, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_LT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "z"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
  };

  return tokens;
}

TEST_CASE("GenerateSubTree: Process Not_Condition") {
  vector<SourceToken> tokens = notConditionInput();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ConditionalExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node != nullptr);
}

TEST_CASE("GenerateSubTree: Process And_Condition") {
  vector<SourceToken> tokens = AndConditionInput();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ConditionalExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node != nullptr);
}

TEST_CASE("GenerateSubTree: Process Or_Condition") {
  vector<SourceToken> tokens = OrConditionInput();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ConditionalExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node != nullptr);
}

TEST_CASE("GenerateSubTree: Process Relational_Expression") {
  vector<SourceToken> tokens = RelationalExpressionInput();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ConditionalExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node != nullptr);
}
