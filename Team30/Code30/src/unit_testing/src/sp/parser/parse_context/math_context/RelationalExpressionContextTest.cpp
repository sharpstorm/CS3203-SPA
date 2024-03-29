#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ast/IASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "sp/parser/conditional_context/contexts/RelationalExpressionContext.h"
#include "sp/parser/entity_context/EntityParser.h"
#include "sp/parser/expression_context/ExpressionParser.h"
#include "sp/parser/conditional_context/ConditionalContextProvider.h"

vector<SourceToken> GTE_Input() {  // (x >= y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_GTE, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> GT_Input() {  // (x > y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_GT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> LTE_Input() {  // (x <= y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_LTE, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> LT_Input() {  // (x < y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_LT, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> EQ_Input() {  // (x == y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> NotEQ_Input() {  // (x != y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_NOT_EQUALS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
      SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, ""),
  };

  return tokens;
}

ASTNodePtr testRelationalParsing(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  EntityParser entParser;
  ExpressionParser exprParser(&entParser);
  ConditionalContextProvider ccp(&exprParser);
  RelationalExpressionContext context(&ccp);
  return context.generateSubtree(&state);
}

TEST_CASE("RelationalExpressionContext: Process GTE_Condition") {
  vector<SourceToken> tokens = GTE_Input();
  auto node = testRelationalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_GTE);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("RelationalExpressionContext: Process GT_Condition") {
  vector<SourceToken> tokens = GT_Input();
  auto node = testRelationalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_GT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("RelationalExpressionContext: Process LTE_Condition") {
  vector<SourceToken> tokens = LTE_Input();
  auto node = testRelationalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_LTE);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("RelationalExpressionContext: Process LT_Condition") {
  vector<SourceToken> tokens = LT_Input();
  auto node = testRelationalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_LT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("RelationalExpressionContext: Process Equal_Condition") {
  vector<SourceToken> tokens = EQ_Input();
  auto node = testRelationalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_EQUALS);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("RelationalExpressionContext: Process Not_Equal_Condition") {
  vector<SourceToken> tokens = NotEQ_Input();
  auto node = testRelationalParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_NOT_EQUALS);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}