#pragma once

#include "Extractor.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/common/ASTNode/StatementListNode.h"
#include "sp/common/ASTNode/statement/AssignNode.h"
#include "sp/common/ASTNode/statement/IfNode.h"
#include "sp/common/ASTNode/statement/PrintNode.h"
#include "sp/common/ASTNode/statement/ReadNode.h"
#include "sp/common/ASTNode/statement/WhileNode.h"

class StatementListNode;
class IfNode;
class ReadNode;

class AbstractExtractor : public Extractor {
 public:
  virtual void visit(StatementListNode node);
  virtual void visit(IfNode node);
  virtual void visit(ReadNode node);
  virtual void visit(AssignNode node);
  virtual void visit(PrintNode node);
  virtual void visit(WhileNode node);

 private:
  PkbWriter* pkbWriter;
};
