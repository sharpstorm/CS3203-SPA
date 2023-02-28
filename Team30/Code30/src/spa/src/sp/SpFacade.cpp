#include "SpFacade.h"
#include "SpDriver.h"

void SpFacade::parseSource(string fileName, PkbWriter* pkbWriter) {
  spDriver.parseSource(fileReader.readFromFile(fileName), pkbWriter);
}

AST SpFacade::parseExpression(string expression) {
  return spDriver.parseExpression(expression);
}
