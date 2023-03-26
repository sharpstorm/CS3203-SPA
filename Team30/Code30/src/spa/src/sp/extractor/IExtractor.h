#pragma once

#include "pkb/writers/PkbWriter.h"

class ProcedureNode;
class StatementListNode;
class IfNode;
class ReadNode;
class PrintNode;
class WhileNode;
class AssignNode;
class CallNode;
class VariableASTNode;
class ConstantASTNode;
class AbstractExpressionNode;
class AbstractConditionalNode;

class IContainerNodeExtractorPart {
 public:
  virtual void visitProcedure(ProcedureNode* node) = 0;
  virtual void visitStmtList(StatementListNode* node) = 0;
  virtual void visitIf(IfNode* node) = 0;
  virtual void visitWhile(WhileNode* node) = 0;

  virtual void leaveProcedure(ProcedureNode* node) = 0;
  virtual void leaveStmtList(StatementListNode* node) = 0;
  virtual void leaveIf(IfNode* node) = 0;
  virtual void leaveWhile(WhileNode* node) = 0;
};

class IBinaryNodeExtractorPart {
 public:
  virtual void visitExpression(AbstractExpressionNode* node) = 0;
  virtual void visitConditional(AbstractConditionalNode* node) = 0;
};

class ILeafNodeExtractorPart {
 public:
  virtual void visitRead(ReadNode* node) = 0;
  virtual void visitPrint(PrintNode* node) = 0;
  virtual void visitAssign(AssignNode* node) = 0;
  virtual void visitCall(CallNode* node) = 0;
  virtual void visitVariable(VariableASTNode* node) = 0;
  virtual void visitConstant(ConstantASTNode* node) = 0;
  virtual void leaveAssign(AssignNode* node) = 0;
};

class IExtractor : virtual public IContainerNodeExtractorPart,
                   virtual public IBinaryNodeExtractorPart,
                   virtual public ILeafNodeExtractorPart {
 public:
  virtual ~IExtractor() = default;
};
