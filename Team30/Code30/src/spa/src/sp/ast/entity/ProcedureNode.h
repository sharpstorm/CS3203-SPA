#pragma once

#include <string>
#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  string toString();
  string getName();
  void leave(Extractor *e);
  void accept(Extractor *e);

 private:
  string procName;
};
