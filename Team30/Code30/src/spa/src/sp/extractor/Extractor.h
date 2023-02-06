#pragma once

#include "pkb/writers/PkbWriter.h"
class StatementListNode;
class IfNode;
class ReadNode;
class PrintNode;
class WhileNode;
class AssignNode;

class Extractor {
 public:
  ~Extractor() = default;
  virtual void visit(StatementListNode node) = 0;
  virtual void visit(IfNode node) = 0;
  virtual void visit(ReadNode node) = 0;
  virtual void visit(PrintNode node) = 0;
  virtual void visit(WhileNode node) = 0;
  virtual void visit(AssignNode node) = 0;
 private:
  PkbWriter* writer;
};