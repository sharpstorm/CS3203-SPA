#pragma once

#include <string>

#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class WhileNode : public StatementASTNode {
 public:
  WhileNode(int lineNumber);
  ~WhileNode() = default;

  string toString();
  void accept(Extractor* e);
  void leave(Extractor* e);
};
