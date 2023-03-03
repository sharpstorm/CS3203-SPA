#include "SpFacade.h"

#include <memory>
#include "SpDriver.h"

using std::unique_ptr;

void SpFacade::parseSource(string fileName, PkbWriter* pkbWriter) {
  spDriver.parseSource(fileReader.readFromFile(fileName), pkbWriter);
}

IASTPtr SpFacade::parseExpression(string expression) {
  AST parsed = spDriver.parseExpression(expression);
  return unique_ptr<IAST>(new AST(parsed));
}
