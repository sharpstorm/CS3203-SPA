#pragma once

#include <string>
#include "sp/ast/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class CallNode : public StatementASTNode {
 public:
  explicit CallNode(int lineNumber, string procName);
  ~CallNode() = default;

  string toString();
  void accept(Extractor* e);

 private:
  string procName;
};
