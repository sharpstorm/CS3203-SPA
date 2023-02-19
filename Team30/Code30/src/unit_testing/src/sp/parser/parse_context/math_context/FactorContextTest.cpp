#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ASTNode/IASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/GrammarContextProvider.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "../../../../../../spa/src/sp/parser/parse_context/math_context/FactorContext.h"

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

TEST_CASE("GenerateSubTree: Process Add_Variables Condition") {
  vector<SourceToken> tokens = plus_Variable_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Subtract_Variables Condition") {
  vector<SourceToken> tokens = minus_Variable_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_VARIABLE);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Add_Integers Condition") {
  vector<SourceToken> tokens = plus_Integers_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_CONSTANT);

  node.reset();
}

TEST_CASE("GenerateSubTree: Process Subtract_Integers Condition") {
  vector<SourceToken> tokens = minus_Integers_Input();
  SourceParseState state(&tokens);
  GrammarContextProvider gcp;
  TermContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node->getType() == ASTNODE_CONSTANT);

  node.reset();
}
