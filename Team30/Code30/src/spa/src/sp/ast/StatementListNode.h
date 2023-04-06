#pragma once

#include <string>
#include "ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class StatementListNode : public ASTNode {
 public:
  StatementListNode();
  void accept(IExtractor* e);
  void leave(IExtractor* e);
};
