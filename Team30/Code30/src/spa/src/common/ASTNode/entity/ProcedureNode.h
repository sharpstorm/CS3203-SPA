#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  string toString();
 private:
  string procName;
};
