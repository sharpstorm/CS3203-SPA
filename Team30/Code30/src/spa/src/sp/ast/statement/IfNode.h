#pragma once

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

class IfNode : public StatementASTNode {
 public:
  explicit IfNode(LineNumber line);
  ~IfNode() = default;

  void accept(IExtractor *e) const override;
  void leave(IExtractor *e) const override;
};
