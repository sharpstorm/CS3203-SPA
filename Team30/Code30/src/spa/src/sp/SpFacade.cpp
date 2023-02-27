#include "SpFacade.h"
#include "SpDriver.h"

void SpFacade::parseSource(string fileName, PkbWriter* pkbWriter) {
  spDriver.parseSource(fileReader.readFromFile(fileName), pkbWriter);
}

void SpFacade::parseExpression(string expression) {
  spDriver.parseExpression(expression);
}
