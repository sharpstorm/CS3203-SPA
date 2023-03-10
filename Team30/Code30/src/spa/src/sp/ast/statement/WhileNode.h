#pragma once

#include <string>

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class WhileNode : public StatementASTNode {
 public:
  explicit WhileNode(int lineNumber);
  ~WhileNode() = default;

  string toString();
  void accept(IExtractor* e);
  void leave(IExtractor* e);
};
