#pragma once

#include <string>
#include <vector>
#include "ASTNode.h"
#include "sp/extractor/AbstractExtractor.h"

using std::string;

class StatementListNode : public ASTNode {
 public:
  StatementListNode();
  ~StatementListNode() = default;
  string toString();
  void accept(const Extractor *e);
};
