#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "sp/parser/conditional_context/ConditionalContextProvider.h"
#include "sp/parser/entity_context/EntityParser.h"

ASTNodePtr testConditionalParsing(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  EntityParser entParser;
  ExpressionParser exprParser(&entParser);
  ConditionalContextProvider ccp(&exprParser);
  ConditionalExpressionContext context(&ccp);
  return context.generateSubtree(&state);
}

vector<SourceToken> notConditionInput() {  // !(x == y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_NOT, ""),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
  };

  return tokens;
}

vector<SourceToken> AndConditionInput() {  // ((x != y) && (x != z))
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

vector<SourceToken> OrConditionInput() {  // ((x == y) || (x != z))
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

vector<SourceToken> RelationalExpressionInput() {  // ((x >= y) || (x < z))
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_GTE, ""),
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

TEST_CASE("ConditionalExpressionContext: Process Not_Condition") {
  vector<SourceToken> tokens = notConditionInput();
  auto node = testConditionalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_NOT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_EQUALS);
  REQUIRE(node->getChild(0)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(0)->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("ConditionalExpressionContext: Process And_Condition") {
  vector<SourceToken> tokens = AndConditionInput();
  auto node = testConditionalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_AND);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_NOT_EQUALS);
  REQUIRE(node->getChild(0)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(0)->getChild(1)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_NOT_EQUALS);
  REQUIRE(node->getChild(1)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("ConditionalExpressionContext: Process Or_Condition") {
  vector<SourceToken> tokens = OrConditionInput();
  auto node = testConditionalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_OR);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_EQUALS);
  REQUIRE(node->getChild(0)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(0)->getChild(1)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_NOT_EQUALS);
  REQUIRE(node->getChild(1)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("ConditionalExpressionContext: Process Relational_Expression") {
  vector<SourceToken> tokens = RelationalExpressionInput();
  auto node = testConditionalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_OR);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_GTE);
  REQUIRE(node->getChild(0)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(0)->getChild(1)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_LT);
  REQUIRE(node->getChild(1)->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}
