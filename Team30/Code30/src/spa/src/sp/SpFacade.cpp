#include "SpFacade.h"

void SpFacade::parseSource(string fileName) {
  parser.parseSource(fileReader.readFromFile(fileName));
}
