#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  ~ProcedureNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  ASTNodeType getType();
 private:
  string procName;
};
