#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ASTNode/IASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/GrammarContextProvider.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "../../../../../../spa/src/sp/parser/parse_context/math_context/ExpressionContext.h"

vector<SourceToken> plus_Variables_Expression_Input() {  // (x + y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_PLUS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> minus_Variables_Expression_Input() {  // (x - y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_MINUS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> plus_Integers_Expression_Input() {  // (5 + 2)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "5"),
      SourceToken(SIMPLE_TOKEN_PLUS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "2"),
  };

  return tokens;
}

vector<SourceToken> minus_Integers_Expression_Input() {  // (10 - 3)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "10"),
      SourceToken(SIMPLE_TOKEN_MINUS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "3"),
  };

  return tokens;
}

TEST_CASE("ExpressionContext: Process Add Variables Condition") {
  vector<SourceToken> tokens = plus_Variables_Expression_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_PLUS);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("ExpressionContext: Process Minus Variables Condition") {
  vector<SourceToken> tokens = minus_Variables_Expression_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_MINUS);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("ExpressionContext: Process Add Integers Condition") {
  vector<SourceToken> tokens = plus_Integers_Expression_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_PLUS);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("ExpressionContext: Process Minus Integers Condition") {
  vector<SourceToken> tokens = minus_Integers_Expression_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  ExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_MINUS);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_CONSTANT);

  node.reset();
}