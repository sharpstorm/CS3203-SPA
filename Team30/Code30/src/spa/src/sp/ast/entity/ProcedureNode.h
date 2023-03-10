#pragma once

#include <string>
#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  string toString();
  string getName();
  void leave(IExtractor *e);
  void accept(IExtractor *e);

 private:
  string procName;
};
