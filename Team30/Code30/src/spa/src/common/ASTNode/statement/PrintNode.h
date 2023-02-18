#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class PrintNode : public StatementASTNode {
 public:
  PrintNode(int lineNumber);
  ~PrintNode() = default;

  string toString();
  void accept(Extractor* e);
};
