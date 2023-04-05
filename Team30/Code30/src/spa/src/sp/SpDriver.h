#pragma once

#include "pkb/writers/PkbWriter.h"
#include "SourceParser.h"
#include "common/parser/ISourceExpressionParser.h"

class SpDriver : public ISourceExpressionParser {
 public:
  void parseSource(const FileData &input, PkbWriter *pkbWriter);
  IASTPtr parseExpression(const SourceExpression &expression) override;

 private:
  SourceParser parser;
};
