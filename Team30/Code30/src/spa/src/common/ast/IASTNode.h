#pragma once

#include <string>

using std::string;

enum ASTNodeType {
  ASTNODE_PROGRAM,
  ASTNODE_PROCEDURE,
  ASTNODE_CALL,
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
  ASTNODE_VARIABLE,
  ASTNODE_CONSTANT,
};

typedef string ASTNodeValue;

class IASTNode {
 public:
  virtual ~IASTNode() = default;
  virtual IASTNode* getChild(int index) const = 0;
  virtual int getChildCount() const = 0;
  virtual ASTNodeType getType() const = 0;
  virtual ASTNodeValue getValue() const = 0;
  virtual bool isEquals(IASTNode *other) const = 0;
};
