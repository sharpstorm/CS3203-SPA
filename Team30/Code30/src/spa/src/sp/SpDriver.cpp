#include "SpDriver.h"
#include "ISourceParser.h"
#include "SourceParser.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/extractor/DesignExtractor.h"

void SpDriver::parseSource(string input, PkbWriter* pkbWriter) {
  DesignExtractor designExtractor(pkbWriter);
  SourceParser sourceParser = SourceParser();
  AST ast = sourceParser.parseSource(input);
  designExtractor.extract(ast);
}
