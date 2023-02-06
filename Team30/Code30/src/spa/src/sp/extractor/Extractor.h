#pragma once

class StatementListNode;
class IfNode;
class ReadNode;

class Extractor {
 public:
  Extractor() = default;
  ~Extractor() = default;
  virtual void visit(StatementListNode node) = 0;
  virtual void visit(IfNode node) = 0;
  virtual void visit(ReadNode node) = 0;
};
