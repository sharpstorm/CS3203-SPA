#pragma once

#include <string>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  ~ProcedureNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);

 private:
  string procName;
};
