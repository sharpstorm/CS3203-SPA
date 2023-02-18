#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class AssignNode : public StatementASTNode {
 public:
  AssignNode(int lineNumber);
  ~AssignNode() = default;

  string toString();
  void accept(Extractor* e);
};
