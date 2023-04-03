#pragma once

#include <string>
#include "sp/ast/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class CallNode : public StatementASTNode {
 public:
  explicit CallNode(LineNumber line, string procName);
  ~CallNode() = default;

  string getName();
  void accept(IExtractor* e) override;

 private:
  string procName;
};
