#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class IfNode : public StatementASTNode {
 public:
  explicit IfNode(int lineNumber);
  ~IfNode() = default;

  string toString() override;
  void accept(IExtractor* e) override;
  void leave(IExtractor* e) override;
};
