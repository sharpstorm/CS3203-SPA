#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class IfNode : public StatementASTNode {
 public:
  explicit IfNode(int lineNumber);
  ~IfNode() = default;

  string toString();
  void accept(Extractor* e);
  void leave(Extractor* e);
};
