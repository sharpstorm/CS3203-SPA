#pragma once

#include <string>
#include <memory>
#include "common/ast/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class ProgramNode : public ASTNode {
 public:
  ProgramNode();
  string toString();
  void leave(Extractor *e);
  void accept(Extractor *e);
};
