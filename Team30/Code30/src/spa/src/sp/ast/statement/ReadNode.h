#pragma once

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

class ReadNode : public StatementASTNode {
 public:
  explicit ReadNode(LineNumber line);
  ~ReadNode() = default;

  void accept(IExtractor* e) const override;
};
