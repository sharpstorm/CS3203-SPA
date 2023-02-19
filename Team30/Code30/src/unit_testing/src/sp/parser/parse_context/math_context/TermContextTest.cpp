#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ASTNode/IASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/GrammarContextProvider.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "../../../../../../spa/src/sp/parser/parse_context/math_context/TermContext.h"

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

TEST_CASE("GenerateSubTree: Process Multiply_Variables Condition") {
  vector<SourceToken> tokens = times_Variable_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_TIMES);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Divide_Variables Condition") {
  vector<SourceToken> tokens = div_Variable_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_DIV);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Modulo_Variables Condition") {
  vector<SourceToken> tokens = mod_Variable_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_MOD);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_VARIABLE);
  REQUIRE(node->getChild(1)->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Multiply_Integers Condition") {
  vector<SourceToken> tokens = times_Integer_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_TIMES);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Divide_Integers Condition") {
  vector<SourceToken> tokens = div_Integer_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_DIV);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Modulo_Integers Condition") {
  vector<SourceToken> tokens = mod_Integer_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_MOD);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);
  REQUIRE(node->getChild(0)->getType() == ASTNODE_CONSTANT);

  node.reset();
}
