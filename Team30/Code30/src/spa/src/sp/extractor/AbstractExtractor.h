#pragma once

#include "IExtractor.h"
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
  void visitProcedure(const ProcedureNode* node) override {}
  void visitStmtList(const StatementListNode* node) override {}
  void visitIf(const IfNode* node) override {}
  void visitWhile(const WhileNode* node) override {}

  void leaveProcedure(const ProcedureNode* node) override {}
  void leaveStmtList(const StatementListNode* node) override {}
  void leaveIf(const IfNode* node) override {}
  void leaveWhile(const WhileNode* node) override {}
};

class AbstractBinaryNodeExtractorPart: virtual public IBinaryNodeExtractorPart {
 public:
  void visitExpression(const AbstractExpressionNode* node) override {}
  void visitConditional(const AbstractConditionalNode* node) override {}
};

class AbstractLeafNodeExtractorPart: virtual public ILeafNodeExtractorPart {
 public:
  void visitRead(const ReadNode* node) override {}
  void visitPrint(const PrintNode* node) override {}
  void visitAssign(const AssignNode* node) override {}
  void visitCall(const CallNode* node) override {}
  void visitVariable(const VariableASTNode* node) override {}
  void visitConstant(const ConstantASTNode* node) override {}
  void leaveAssign(const AssignNode* node) override {}
};

class AbstractExtractor : public IExtractor,
                          public AbstractContainerNodeExtractorPart,
                          public AbstractBinaryNodeExtractorPart,
                          public AbstractLeafNodeExtractorPart {};
