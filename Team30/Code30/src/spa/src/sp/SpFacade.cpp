#include "SpFacade.h"

#include <memory>
#include "SpDriver.h"

using std::unique_ptr;

void SpFacade::parseSource(string fileName, PkbWriter* pkbWriter) {
  spDriver.parseSource(fileReader.readFromFile(fileName), pkbWriter);
}

IASTPtr SpFacade::parseExpression(string expression) {
  return spDriver.parseExpression(expression);
}
