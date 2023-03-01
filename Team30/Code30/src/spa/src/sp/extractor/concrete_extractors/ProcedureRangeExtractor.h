#pragma once

#include <memory>

#include "sp/extractor/AbstractExtractor.h"
#include "common/ast/entity/ProcedureNode.h"

using std::shared_ptr;

class ProcedureRangeExtractor : public AbstractExtractor {
 public:
  explicit ProcedureRangeExtractor(PkbWriter* pkbWriter);
  void visit(ProcedureNode* node);
 private:
  void addProcedureRange(string name, int start, int end);
  PkbWriter* pkbWriter;
};
