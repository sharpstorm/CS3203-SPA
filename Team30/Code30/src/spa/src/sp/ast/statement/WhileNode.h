#pragma once

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

class WhileNode : public StatementASTNode {
 public:
  explicit WhileNode(LineNumber line);
  ~WhileNode() = default;

  void accept(IExtractor *e) const override;
  void leave(IExtractor *e) const override;
};
