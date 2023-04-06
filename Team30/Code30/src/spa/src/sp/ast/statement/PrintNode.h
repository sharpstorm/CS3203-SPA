#pragma once

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

class PrintNode : public StatementASTNode {
 public:
  explicit PrintNode(LineNumber line);
  ~PrintNode() = default;

  void accept(IExtractor* e) const override;
};
