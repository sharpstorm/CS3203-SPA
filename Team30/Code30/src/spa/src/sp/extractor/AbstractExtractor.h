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
#include "common/ASTNode/statement/CallNode.h"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/ConstantASTNode.h"
#include "common/ASTNode/math/math_operand/AbstractMathNode.h"
#include "common/ASTNode/math/conditional_operand/AbstractConditionalNode.h"

class AbstractExtractor : public Extractor {
 public:
  virtual void visit(StatementListNode* node) {}
  virtual void visit(IfNode* node) {}
  virtual void visit(ReadNode* node) {}
  virtual void visit(AssignNode* node) {}
  virtual void visit(PrintNode* node) {}
  virtual void visit(WhileNode* node) {}
  virtual void visit(CallNode* node) {}
  virtual void visit(VariableASTNode* node) {}
  virtual void visit(ConstantASTNode* node) {}
  virtual void visit(AbstractMathNode* node) {}
  virtual void visit(AbstractConditionalNode* node) {}
  virtual void leave(StatementListNode* node) {}
  virtual void leave(IfNode* node) {}
  virtual void leave(ReadNode* node) {}
  virtual void leave(AssignNode* node) {}
  virtual void leave(PrintNode* node) {}
  virtual void leave(WhileNode* node) {}
  virtual void leave(CallNode* node) {}
  virtual void leave(VariableASTNode* node) {}
  virtual void leave(ConstantASTNode* node) {}
  virtual void leave(AbstractMathNode* node) {}
  virtual void leave(AbstractConditionalNode* node) {}

 private:
  PkbWriter* pkbWriter;
};
