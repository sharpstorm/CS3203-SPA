#include "SpDriver.h"

#include <memory>

#include "ISourceParser.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/extractor/DesignExtractor.h"
#include "pkb/errors/PKBError.h"

using std::unique_ptr;

void SpDriver::parseSource(string input, PkbWriter* pkbWriter) {
  DesignExtractor designExtractor(pkbWriter);
  AST ast = parser.parseSource(input);
  designExtractor.extract(ast);
  try {
    pkbWriter->runPostProcessor();
  } catch (PKBError e) {
    throw SPError(e.what());
  }
}

IASTPtr SpDriver::parseExpression(string expression) {
  AST parsed = parser.parseExpression(expression);
  return unique_ptr<IAST>(new AST(parsed));
}
