#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class CallNode : public StatementASTNode {
 public:
  explicit CallNode(string procName);
  ~CallNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  void leave(shared_ptr<Extractor> e);
 private:
  string procName;
};
