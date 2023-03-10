#include "SpDriver.h"

#include <memory>

#include "ISourceParser.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/extractor/DesignExtractor.h"

using std::unique_ptr;

void SpDriver::parseSource(string input, PkbWriter* pkbWriter) {
  DesignExtractor designExtractor(pkbWriter);
  ASTPtr ast = parser.parseSource(input);
  designExtractor.extract(ast.get());
  pkbWriter->runPostProcessor();
}

IASTPtr SpDriver::parseExpression(string expression) {
  return parser.parseExpression(expression);
}
