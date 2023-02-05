#include "SpDriver.h"
#include "ISourceParser.h"
#include "SourceParser.h"

void SpDriver::parseSource(string input) {
  ISourceParser iSourceParser = SourceParser();
  iSourceParser.parseSource(input);
}
