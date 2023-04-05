#pragma once

#include "ISp.h"
#include "common/parser/ISourceExpressionParser.h"
#include "SpDriver.h"

class SpFacade : public ISp, public ISourceExpressionParser {
 public:
  void parseSource(const FilePath &inputPath, PkbWriter *pkbWriter) override;
  IASTPtr parseExpression(const SourceExpression &expression) override;

 private:
  SpDriver spDriver;
};
