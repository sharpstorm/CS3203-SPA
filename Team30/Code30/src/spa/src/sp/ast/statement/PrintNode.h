#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class PrintNode : public StatementASTNode {
 public:
  explicit PrintNode(int lineNumber);
  ~PrintNode() = default;

  string toString() override;
  void accept(IExtractor* e) override;
};
