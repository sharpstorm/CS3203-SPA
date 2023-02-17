#pragma once
#include <memory>
#include <string>
#include "ASTNode.h"

using std::shared_ptr, std::vector, std::string;

enum ASTNodeType {
  ASTNODE_PROCEDURE,
  ASTNODE_STMTLST,
  ASTNODE_PRINT,
  ASTNODE_ASSIGN,
  ASTNODE_READ,
  ASTNODE_WHILE,
  ASTNODE_IF,
  ASTNODE_PLUS,
  ASTNODE_MINUS,
  ASTNODE_TIMES,
  ASTNODE_DIV,
  ASTNODE_MOD,
  ASTNODE_GT,
  ASTNODE_GTE,
  ASTNODE_LT,
  ASTNODE_LTE,
  ASTNODE_NOT_EQUALS,
  ASTNODE_EQUALS,
  ASTNODE_NOT,
  ASTNODE_AND,
  ASTNODE_OR,
  ASTNODE_OPERAND,
  ASTNODE_VARIABLE,
  ASTNODE_CONSTANT,
};

class IASTNode {
 public:
  virtual shared_ptr<IASTNode> getChild(int i) = 0;
  virtual ASTNodeType getType() = 0;
  virtual string getValue() = 0;
  virtual bool isEquals(IASTNode& other) = 0;
};
