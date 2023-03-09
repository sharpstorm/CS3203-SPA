#pragma once

#include <memory>
#include <string>

#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

using std::shared_ptr, std::string;

class ProcedureRangeExtractor : public AbstractExtractor {
 public:
  explicit ProcedureRangeExtractor(PkbWriter* pkbWriter);
  void visitProcedure(ProcedureNode* node) override;
  void visitRead(ReadNode* node) override;
  void visitPrint(PrintNode* node) override;
  void visitWhile(WhileNode* node) override;
  void visitIf(IfNode* node) override;
  void visitAssign(AssignNode* node) override;
  void visitCall(CallNode* node) override;
 private:
  void addProcedureRange(const string &name, int start, int end);
  PkbWriter* pkbWriter;
  int lineNumberCache;
};
