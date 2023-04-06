#pragma once

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

class AssignNode : public StatementASTNode {
 public:
  explicit AssignNode(LineNumber line);
  ~AssignNode() = default;

  void accept(IExtractor *e) const override;
  void leave(IExtractor *e) const override;
};
