#include <memory>
#include <string>

#include "../../../../../../../lib/catch.hpp"
#include "../../../../../../spa/src/common/ASTNode/BinaryASTNode.h"
#include "../../../../../../spa/src/sp/parser/GrammarContextProvider.h"
#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
#include "../../../../../../spa/src/sp/parser/parse_context/math_context/ConditionalExpressionContext.h"
#include "../../SourceParserStub.cpp"

SourceParseState executeParse(string input) {
  SourceParserStub parser;
  return parser.parseSource_AndGetState(input);
}

TEST_CASE("GenerateSubTree: ProcessNotCondition") {
  string input = "!(num1 || num2)";
  SourceParseState state = executeParse(input);
  GrammarContextProvider gcp;
  ConditionalExpressionContext context(&gcp);
  shared_ptr<ASTNode> node = context.generateSubtree(&state);

  REQUIRE(node != nullptr);
}
