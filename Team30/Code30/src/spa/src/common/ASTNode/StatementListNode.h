#pragma once

#include <string>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class StatementListNode : public ASTNode {
 public:
  StatementListNode();
  string toString();
  void accept(Extractor* e);
};
