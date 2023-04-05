#include "SpDriver.h"

#include "sp/extractor/DesignExtractor.h"
#include "pkb/errors/PKBError.h"

void SpDriver::parseSource(const FileData &input, PkbWriter* pkbWriter) {
  DesignExtractor designExtractor(pkbWriter);
  ASTPtr ast = parser.parseSource(input);
  designExtractor.extract(ast.get());
  try {
    pkbWriter->runPostProcessor();
  } catch (PKBError e) {
    throw SPError(e.what());
  }
}

IASTPtr SpDriver::parseExpression(const SourceExpression &expression) {
  return parser.parseExpression(expression);
}
