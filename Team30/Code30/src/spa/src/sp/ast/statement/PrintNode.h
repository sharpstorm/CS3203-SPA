#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class PrintNode : public StatementASTNode {
 public:
  explicit PrintNode(LineNumber line);
  ~PrintNode() = default;

  void accept(IExtractor* e) override;
};
