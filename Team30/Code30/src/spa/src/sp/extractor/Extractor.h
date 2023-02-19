#pragma once

#include "pkb/writers/PkbWriter.h"

class StatementListNode;
class IfNode;
class ReadNode;
class PrintNode;
class WhileNode;
class AssignNode;
class CallNode;
class VariableASTNode;
class ConstantASTNode;
class AbstractMathNode;
class AbstractConditionalNode;

class Extractor {
 public:
  virtual ~Extractor() = default;
  virtual void visit(StatementListNode* node) = 0;
  virtual void visit(IfNode* node) = 0;
  virtual void visit(ReadNode* node) = 0;
  virtual void visit(PrintNode* node) = 0;
  virtual void visit(WhileNode* node) = 0;
  virtual void visit(AssignNode* node) = 0;
  virtual void visit(CallNode* node) = 0;
  virtual void visit(VariableASTNode* node) = 0;
  virtual void visit(ConstantASTNode* node) = 0;
  virtual void visit(AbstractMathNode* node) = 0;
  virtual void visit(AbstractConditionalNode* node) = 0;
  virtual void leave(IfNode* node) = 0;
  virtual void leave(ReadNode* node) = 0;
  virtual void leave(PrintNode* node) = 0;
  virtual void leave(WhileNode* node) = 0;
  virtual void leave(AssignNode* node) = 0;
  virtual void leave(CallNode* node) = 0;
  virtual void leave(VariableASTNode* node) = 0;
  virtual void leave(ConstantASTNode* node) = 0;
  virtual void leave(AbstractMathNode* node) = 0;
  virtual void leave(AbstractConditionalNode* node) = 0;

 private:
  PkbWriter* writer;
};
