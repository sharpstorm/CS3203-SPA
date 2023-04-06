#pragma once

#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/SPTypes.h"

class PatternExtractor : public AbstractExtractor {
 public:
  explicit PatternExtractor(PkbWriter *pkbWriter);
  void leaveAssign(const AssignNode *node) override;
  void leaveWhile(const WhileNode *node) override;
  void leaveIf(const IfNode *node) override;
 private:
  void addPattern(LineNumber x, IASTNode *node);
  void leave(ASTNodeType type, const StatementASTNode *node);
  PkbWriter *pkbWriter;
};
