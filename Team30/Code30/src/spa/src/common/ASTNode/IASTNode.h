#pragma once
#include <memory>
#include <vector>
#include <string>

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
  bool operator==(const IASTNode& other) const {
    if (type == ASTNODE_VARIABLE || type == ASTNODE_CONSTANT) {
      return type == other.type && value == other.value;
    }
    return type == other.type;
  }
 protected:
  ASTNodeType type;
  string value;
};
