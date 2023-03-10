#pragma once

#include <string>
#include "sp/ast/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class CallNode : public StatementASTNode {
 public:
  explicit CallNode(int lineNumber, string procName);
  ~CallNode() = default;

  string getName();
  string toString() override;
  void accept(IExtractor* e) override;

 private:
  string procName;
};
