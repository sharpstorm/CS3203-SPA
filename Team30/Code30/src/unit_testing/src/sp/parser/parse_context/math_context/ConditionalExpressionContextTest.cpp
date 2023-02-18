#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ASTNode/BinaryASTNode.h"
#include "../../../../../../spa/src/sp/common/SourceToken.h"
#include "../../../../../../spa/src/sp/parser/GrammarContextProvider.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "../../../../../../spa/src/sp/parser/parse_context/math_context/ConditionalExpressionContext.h"
#include "../../SourceParserStub.cpp"

SourceParseState executeParse(vector<SourceToken> tokens) {
  SourceParserStub parser;
  return parser.parseTokens_AndGetState(tokens);
}

vector<SourceToken> notConditionInput() {  //!(x && 1)
  vector<SourceToken> tokens{SourceToken(SIMPLE_TOKEN_NOT, ""),
                             SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_LEFT, ""),
                             SourceToken(SIMPLE_TOKEN_VARIABLE, "x"),
                             SourceToken(SIMPLE_TOKEN_AND, ""),
                             SourceToken(SIMPLE_TOKEN_VARIABLE, "y"),
                             SourceToken(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT, "")};

  return tokens;
}

TEST_CASE("GenerateSubTree: ProcessNotCondition") {
  vector<SourceToken> tokens = notConditionInput();
  SourceParseState state = executeParse(tokens);
  GrammarContextProvider gcp;
  ConditionalExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node != nullptr);
}
