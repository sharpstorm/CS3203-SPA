#pragma once

#include "Extractor.h"
#include "sp/common/ASTNode/StatementListNode.h"
#include "sp/common/ASTNode/statement/IfNode.h"
#include "sp/common/ASTNode/statement/ReadNode.h"

class StatementListNode;
class IfNode;
class ReadNode;

class AbstractExtractor : public Extractor {
 public:
  virtual void visit(StatementListNode node);
  virtual void visit(IfNode node);
  virtual void visit(ReadNode node);
};
