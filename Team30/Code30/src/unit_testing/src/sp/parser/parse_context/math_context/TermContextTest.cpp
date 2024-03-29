#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ast/IASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "sp/parser/expression_context/contexts/TermContext.h"
#include "sp/parser/entity_context/EntityParser.h"
#include "sp/parser/expression_context/ExpressionContextProvider.h"

vector<SourceToken> times_Variable_Input() {  // (x * y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_TIMES, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> times_Integer_Input() {  // (4 * 7)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "4"),
      SourceToken(SIMPLE_TOKEN_TIMES, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "7"),
  };

  return tokens;
}

vector<SourceToken> div_Variable_Input() {  // (x / y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_DIV, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> div_Integer_Input() {  // (8 / 2)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "8"),
      SourceToken(SIMPLE_TOKEN_DIV, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "2"),
  };

  return tokens;
}

vector<SourceToken> mod_Variable_Input() {  // (x % y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_MOD, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> mod_Integer_Input() {  // (15 % 5)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "15"),
      SourceToken(SIMPLE_TOKEN_MOD, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "3"),
  };

  return tokens;
}

ASTNodePtr testTermParsing(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  EntityParser entParser;
  ExpressionContextProvider ecp(&entParser);
  TermContext context(&ecp);
  return context.generateSubtree(&state);
}

TEST_CASE("TermContext: Process Multiply_Variables Condition") {
  vector<SourceToken> tokens = times_Variable_Input();
  auto node = testTermParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_TIMES);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("TermContext: Process Divide_Variables Condition") {
  vector<SourceToken> tokens = div_Variable_Input();
  auto node = testTermParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_DIV);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("TermContext: Process Modulo_Variables Condition") {
  vector<SourceToken> tokens = mod_Variable_Input();
  auto node = testTermParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_MOD);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("TermContext: Process Multiply_Integers Condition") {
  vector<SourceToken> tokens = times_Integer_Input();
  auto node = testTermParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_TIMES);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("TermContext: Process Divide_Integers Condition") {
  vector<SourceToken> tokens = div_Integer_Input();
  auto node = testTermParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_DIV);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("TermContext: Process Modulo_Integers Condition") {
  vector<SourceToken> tokens = mod_Integer_Input();
  auto node = testTermParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_MOD);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);

  node.reset();
}
