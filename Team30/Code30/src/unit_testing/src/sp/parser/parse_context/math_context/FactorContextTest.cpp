#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ast/IASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "sp/parser/expression_context/contexts/FactorContext.h"
#include "sp/parser/entity_context/EntityParser.h"
#include "sp/parser/expression_context/ExpressionContextProvider.h"

vector<SourceToken> plus_Variable_Input() {  // (x + y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_PLUS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> minus_Variable_Input() {  // (x - y)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
      SourceToken(SIMPLE_TOKEN_MINUS, ""),
      SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
  };

  return tokens;
}

vector<SourceToken> plus_Integers_Input() {  // (4 + 7)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "4"),
      SourceToken(SIMPLE_TOKEN_PLUS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "7"),
  };

  return tokens;
}

vector<SourceToken> minus_Integers_Input() {  // (9 - 5)
  vector<SourceToken> tokens = vector<SourceToken>{
      SourceToken(SIMPLE_TOKEN_INTEGER, "9"),
      SourceToken(SIMPLE_TOKEN_MINUS, ""),
      SourceToken(SIMPLE_TOKEN_INTEGER, "5"),
  };

  return tokens;
}

ASTNodePtr testFactorParsing(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  EntityParser entParser;
  ExpressionContextProvider ecp(&entParser);
  FactorContext context(&ecp, &entParser);
  return context.generateSubtree(&state);
}


TEST_CASE("FactorContext: Process Add_Variables Condition") {
  vector<SourceToken> tokens = plus_Variable_Input();
  auto node = testFactorParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("FactorContext: Process Subtract_Variables Condition") {
  vector<SourceToken> tokens = minus_Variable_Input();
  auto node = testFactorParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("FactorContext: Process Add_Integers Condition") {
  vector<SourceToken> tokens = plus_Integers_Input();
  auto node = testFactorParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("FactorContext: Process Subtract_Integers Condition") {
  vector<SourceToken> tokens = minus_Integers_Input();
  auto node = testFactorParsing(&tokens);

  REQUIRE(node->getType() == ASTNODE_CONSTANT);

  node.reset();
}
