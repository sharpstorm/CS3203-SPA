#include "SpFacade.h"

#include "SpDriver.h"
#include "FileReader.h"

void SpFacade::parseSource(const FilePath &fileName, PkbWriter *pkbWriter) {
  FileDataPtr fileData = FileReader::readFromFile(fileName);
  spDriver.parseSource(*fileData, pkbWriter);
}

IASTPtr SpFacade::parseExpression(const SourceExpression &expression) {
  return spDriver.parseExpression(expression);
}
