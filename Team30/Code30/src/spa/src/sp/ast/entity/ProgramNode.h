#pragma once

#include <string>
#include <memory>
#include "../ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class ProgramNode : public ASTNode {
 public:
  ProgramNode();
  string toString();
};
