#pragma once

#include <memory>
#include <string>
#include "sp/extractor/AbstractExtractor.h"
#include "common/ast/entity/ProcedureNode.h"

using std::shared_ptr;

class ProcedureRangeExtractor : public AbstractExtractor {
 public:
  explicit ProcedureRangeExtractor(PkbWriter* pkbWriter);
  void visit(ProcedureNode* node);
  void visit(ReadNode* node);
  void visit(PrintNode* node);
  void visit(WhileNode* node);
  void visit(IfNode* node);
  void visit(AssignNode* node);
  void visit(CallNode* node);
 private:
  void addProcedureRange(string name, int start, int end);
  PkbWriter* pkbWriter;
  int lineNumberCache;
};
