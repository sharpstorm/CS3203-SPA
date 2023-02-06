#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class StatementListNode : public ASTNode {
 public:
  StatementListNode();
  ~StatementListNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
};
