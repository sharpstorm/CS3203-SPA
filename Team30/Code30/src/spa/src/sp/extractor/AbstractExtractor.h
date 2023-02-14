#pragma once

#include "Extractor.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "common/ASTNode/StatementListNode.h"
#include "common/ASTNode/statement/AssignNode.h"
#include "common/ASTNode/statement/IfNode.h"
#include "common/ASTNode/statement/PrintNode.h"
#include "common/ASTNode/statement/ReadNode.h"
#include "common/ASTNode/statement/WhileNode.h"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/ConstantASTNode.h"


class AbstractExtractor : public Extractor {
 public:
  virtual void visit(StatementListNode node);
  virtual void visit(IfNode node);
  virtual void visit(ReadNode node);
  virtual void visit(AssignNode node);
  virtual void visit(PrintNode node);
  virtual void visit(WhileNode node);
  virtual void visit(VariableASTNode node);
  virtual void visit(ConstantASTNode node);

 private:
  PkbWriter* pkbWriter;
};
