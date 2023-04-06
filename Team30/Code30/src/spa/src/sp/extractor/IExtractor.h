#pragma once

#include <memory>
#include "pkb/writers/PkbWriter.h"

using std::unique_ptr;

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
  virtual void visitProcedure(const ProcedureNode* node) = 0;
  virtual void visitStmtList(const StatementListNode* node) = 0;
  virtual void visitIf(const IfNode* node) = 0;
  virtual void visitWhile(const WhileNode* node) = 0;

  virtual void leaveProcedure(const ProcedureNode* node) = 0;
  virtual void leaveStmtList(const StatementListNode* node) = 0;
  virtual void leaveIf(const IfNode* node) = 0;
  virtual void leaveWhile(const WhileNode* node) = 0;
};

class IBinaryNodeExtractorPart {
 public:
  virtual void visitExpression(const AbstractExpressionNode* node) = 0;
  virtual void visitConditional(const AbstractConditionalNode* node) = 0;
};

class ILeafNodeExtractorPart {
 public:
  virtual void visitRead(const ReadNode* node) = 0;
  virtual void visitPrint(const PrintNode* node) = 0;
  virtual void visitAssign(const AssignNode* node) = 0;
  virtual void visitCall(const CallNode* node) = 0;
  virtual void visitVariable(const VariableASTNode* node) = 0;
  virtual void visitConstant(const ConstantASTNode* node) = 0;
  virtual void leaveAssign(const AssignNode* node) = 0;
};

class IExtractor : virtual public IContainerNodeExtractorPart,
                   virtual public IBinaryNodeExtractorPart,
                   virtual public ILeafNodeExtractorPart {
 public:
  virtual ~IExtractor() = default;
};

typedef unique_ptr<IExtractor> IExtractorPtr;
