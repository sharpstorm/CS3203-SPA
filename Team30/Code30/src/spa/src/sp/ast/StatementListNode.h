#pragma once

#include "ASTNode.h"
#include "sp/extractor/IExtractor.h"

class StatementListNode : public ASTNode {
 public:
  StatementListNode();
  void accept(IExtractor *e) const override;
  void leave(IExtractor *e) const override;
};
