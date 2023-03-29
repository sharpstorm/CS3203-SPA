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
  void visitProcedure(ProcedureNode* node) override {}
  void visitStmtList(StatementListNode* node) override {}
  void visitIf(IfNode* node) override {}
  void visitWhile(WhileNode* node) override {}

  void leaveProcedure(ProcedureNode* node) override {}
  void leaveStmtList(StatementListNode* node) override {}
  void leaveIf(IfNode* node) override {}
  void leaveWhile(WhileNode* node) override {}
};

class AbstractBinaryNodeExtractorPart: virtual public IBinaryNodeExtractorPart {
 public:
  void visitExpression(AbstractExpressionNode* node) override {}
  void visitConditional(AbstractConditionalNode* node) override {}
};

class AbstractLeafNodeExtractorPart: virtual public ILeafNodeExtractorPart {
 public:
  void visitRead(ReadNode* node) override {}
  void visitPrint(PrintNode* node) override {}
  void visitAssign(AssignNode* node) override {}
  void visitCall(CallNode* node) override {}
  void visitVariable(VariableASTNode* node) override {}
  void visitConstant(ConstantASTNode* node) override {}
  void leaveAssign(AssignNode* node) override {}
};

class AbstractExtractor : public IExtractor,
                          public AbstractContainerNodeExtractorPart,
                          public AbstractBinaryNodeExtractorPart,
                          public AbstractLeafNodeExtractorPart {};
