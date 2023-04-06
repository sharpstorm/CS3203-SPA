#pragma once

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"
#include "sp/SPTypes.h"

class CallNode : public StatementASTNode {
 public:
  explicit CallNode(LineNumber line, const ProcedureName &procName);
  ~CallNode() = default;

  ProcedureName getName() const;
  void accept(IExtractor *e) const override;

 private:
  ProcedureName procName;
};
