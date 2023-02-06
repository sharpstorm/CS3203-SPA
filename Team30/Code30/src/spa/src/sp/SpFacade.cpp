#include "SpFacade.h"
#include "SpDriver.h"

void SpFacade::parseSource(string fileName, PkbWriter* pkbWriter) {
  FileReader fileReader;
  SpDriver spDriver;
  spDriver.parseSource(fileReader.readFromFile(fileName), pkbWriter);
}
