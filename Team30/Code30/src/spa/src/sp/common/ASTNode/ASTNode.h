#pragma once

#include <vector>
#include <string>
#include <memory>

using std::vector, std::string, std::shared_ptr;

enum AST_NODE_TYPE {
    AST_NODE_PROCEDURE,

    AST_NODE_ASSIGN,
    AST_NODE_PRINT,
    AST_NODE_READ,
    AST_NODE_WHILE,
    AST_NODE_IF,
    AST_NODE_THEN,

    AST_NODE_MATH,
    AST_NODE_REL_EXPR,
    AST_NODE_COND_EXPR,

    AST_NODE_VARIABLE,
    AST_NODE_CONST,
};

class ASTNode {
 public:
  virtual ~ASTNode() = default;
  virtual void accept() = 0;
  virtual string toString() = 0;
  virtual AST_NODE_TYPE getType() = 0;
  vector<shared_ptr<ASTNode>> getChildren();
  void setChild(int index, shared_ptr<ASTNode> node);
  void addChild(shared_ptr<ASTNode> node);

 protected:
  vector<shared_ptr<ASTNode>> children;
};
