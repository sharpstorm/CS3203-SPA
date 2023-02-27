#include "SpDriver.h"
#include "ISourceParser.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/extractor/DesignExtractor.h"

void SpDriver::parseSource(string input, PkbWriter* pkbWriter) {
  DesignExtractor designExtractor(pkbWriter);
  AST ast = parser.parseSource(input);
  designExtractor.extract(ast);
}

AST SpDriver::parseExpression(string expression) {
  return parser.parseExpression(expression);
}
