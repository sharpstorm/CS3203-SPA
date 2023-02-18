#include <string>

#include "../../../../../../spa/src/sp/parser/SourceParseState.h"
SourceParseState executeParse(string input) {
  SourceParserStub parser;
  return parser.parseSource_AndGetState(input);
}

TEST_CASE("GenerateSubTree: ProcessNotCondition") {
  string input = "!(num1 < num2)";
  executeParse(input);
}
