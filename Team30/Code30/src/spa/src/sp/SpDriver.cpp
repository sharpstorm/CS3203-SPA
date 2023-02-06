#include "SpDriver.h"
#include "ISourceParser.h"
#include "SourceParser.h"

void SpDriver::parseSource(string input) {
  SourceParser sourceParser = SourceParser();
  sourceParser.parseSource(input);
}
