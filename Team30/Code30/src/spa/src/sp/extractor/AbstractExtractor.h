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

class AbstractContainerNodeExtractorPart:
    virtual public IContainerNodeExtractorPart {
 public:
  virtual void visitProcedure(ProcedureNode* node) override {}
  virtual void visitStmtList(StatementListNode* node) override {}
  virtual void visitIf(IfNode* node) override {}
  virtual void visitWhile(WhileNode* node) override {}

  virtual void leaveProcedure(ProcedureNode* node) override {}
  virtual void leaveStmtList(StatementListNode* node) override {}
  virtual void leaveIf(IfNode* node) override {}
  virtual void leaveWhile(WhileNode* node) override {}
};

class AbstractBinaryNodeExtractorPart: virtual public IBinaryNodeExtractorPart {
 public:
  virtual void visitExpression(AbstractExpressionNode* node) override {}
  virtual void visitConditional(AbstractConditionalNode* node) override {}
};

class AbstractLeafNodeExtractorPart: virtual public ILeafNodeExtractorPart {
 public:
  virtual void visitRead(ReadNode* node) override {}
  virtual void visitPrint(PrintNode* node) override {}
  virtual void visitAssign(AssignNode* node) override {}
  virtual void visitCall(CallNode* node) override {}
  virtual void visitVariable(VariableASTNode* node) override {}
  virtual void visitConstant(ConstantASTNode* node) override {}
};

class AbstractExtractor : public Extractor,
                          public AbstractContainerNodeExtractorPart,
                          public AbstractBinaryNodeExtractorPart,
                          public AbstractLeafNodeExtractorPart {};
