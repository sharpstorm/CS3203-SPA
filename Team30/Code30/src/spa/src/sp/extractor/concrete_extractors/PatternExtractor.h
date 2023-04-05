#pragma once

#include <memory>

#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"

using std::shared_ptr;

class PatternExtractor : public AbstractExtractor {
 public:
  explicit PatternExtractor(PkbWriter* pkbWriter);
  void leaveAssign(AssignNode* node) override;
  void leaveWhile(WhileNode* node) override;
  void leaveIf(IfNode* node) override;
 private:
  void addPattern(LineNumber x, IASTNode* node);
  void leave(ASTNodeType type, StatementASTNode *node);
  PkbWriter* pkbWriter;
};
