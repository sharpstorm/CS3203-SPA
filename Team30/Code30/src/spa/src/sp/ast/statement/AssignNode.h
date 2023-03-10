#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class AssignNode : public StatementASTNode {
 public:
  explicit AssignNode(int lineNumber);
  ~AssignNode() = default;

  string toString() override;
  void accept(IExtractor* e) override;
};
