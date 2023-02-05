#include "SpFacade.h"
#include "FileReader.h"

void SpFacade::parseSource(string fileName) {
  FileReader fileReader;
  parseSource(fileReader.readFromFile(fileName));
}
