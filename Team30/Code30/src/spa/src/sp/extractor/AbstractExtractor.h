#pragma once

#include "Extractor.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

#include "sp/ast/statement/AssignNode.h"
#include "sp/ast/statement/IfNode.h"
#include "sp/ast/statement/PrintNode.h"
#include "sp/ast/statement/ReadNode.h"
#include "sp/ast/statement/WhileNode.h"
#include "sp/ast/statement/CallNode.h"

#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/ProcedureNode.h"
#include "sp/ast/StatementListNode.h"

#include "sp/ast/expression_operand/AbstractExpressionNode.h"
#include "sp/ast/conditional_operand/AbstractConditionalNode.h"

class AbstractExtractor : public Extractor {
 public:
  virtual void visit(ProcedureNode* node) {}
  virtual void visit(StatementListNode* node) {}
  virtual void visit(IfNode* node) {}
  virtual void visit(ReadNode* node) {}
  virtual void visit(AssignNode* node) {}
  virtual void visit(PrintNode* node) {}
  virtual void visit(WhileNode* node) {}
  virtual void visit(CallNode* node) {}
  virtual void visit(VariableASTNode* node) {}
  virtual void visit(ConstantASTNode* node) {}
  virtual void visit(AbstractExpressionNode* node) {}
  virtual void visit(AbstractConditionalNode* node) {}

  virtual void leave(ProcedureNode* node) {}
  virtual void leave(StatementListNode* node) {}
  virtual void leave(IfNode* node) {}
  virtual void leave(ReadNode* node) {}
  virtual void leave(AssignNode* node) {}
  virtual void leave(PrintNode* node) {}
  virtual void leave(WhileNode* node) {}
  virtual void leave(CallNode* node) {}
  virtual void leave(VariableASTNode* node) {}
  virtual void leave(ConstantASTNode* node) {}
  virtual void leave(AbstractExpressionNode* node) {}
  virtual void leave(AbstractConditionalNode* node) {}

 private:
  PkbWriter* pkbWriter;
};
