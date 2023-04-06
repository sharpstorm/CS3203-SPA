#pragma once

#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"
#include "sp/SPTypes.h"

typedef int ProcedureStmtIndex;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(const ProcedureName &name);
  void leave(IExtractor *e) const override;
  void accept(IExtractor *e) const override;

  ProcedureName getName() const;
  ASTNode* getChildStatement(const ProcedureStmtIndex &statementIndex) const;

 private:
  ProcedureName procName;
};
