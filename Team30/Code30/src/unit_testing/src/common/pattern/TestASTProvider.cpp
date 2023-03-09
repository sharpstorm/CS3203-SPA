#include "TestASTProvider.h"
#include "sp/ast/ASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/TimesASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/AST.h"

#include <memory>

using std::make_unique;

TestASTProvider::TestASTProvider() {
  buildBalancedTree();
  buildLeftHeavyTree();
  buildRightHeavyTree();
}

void TestASTProvider::buildBalancedTree() {
  // x * y + x * z;
  ASTNodePtr xNode1 = make_unique<VariableASTNode>("x");
  ASTNodePtr xNode2 = make_unique<VariableASTNode>("x");
  ASTNodePtr yNode = make_unique<VariableASTNode>("y");
  ASTNodePtr zNode = make_unique<VariableASTNode>("z");

  auto timesNode1 = make_unique<TimesASTNode>();
  timesNode1->setLeftChild(std::move(xNode1));
  timesNode1->setRightChild(std::move(yNode));

  auto timesNode2 = make_unique<TimesASTNode>();
  timesNode2->setLeftChild(std::move(xNode2));
  timesNode2->setRightChild(std::move(zNode));

  auto plusNode = make_unique<PlusASTNode>();
  plusNode->setLeftChild(std::move(timesNode1));
  plusNode->setRightChild(std::move(timesNode2));

  balancedTree = make_unique<AST>(std::move(plusNode));
}

void TestASTProvider::buildLeftHeavyTree() {
  // x * y + z;
  ASTNodePtr xNode = make_unique<VariableASTNode>("x");
  ASTNodePtr yNode = make_unique<VariableASTNode>("y");
  ASTNodePtr zNode = make_unique<VariableASTNode>("z");

  auto timesNode = make_unique<TimesASTNode>();
  timesNode->setLeftChild(std::move(xNode));
  timesNode->setRightChild(std::move(yNode));

  auto plusNode = make_unique<PlusASTNode>();
  plusNode->setLeftChild(std::move(timesNode));
  plusNode->setRightChild(std::move(zNode));

  leftHeavyTree = make_unique<AST>(std::move(plusNode));
}

void TestASTProvider::buildRightHeavyTree() {
  ASTNodePtr xNode = make_unique<VariableASTNode>("x");
  ASTNodePtr yNode = make_unique<VariableASTNode>("y");
  ASTNodePtr zNode = make_unique<VariableASTNode>("z");

  auto timesNode = make_unique<TimesASTNode>();
  timesNode->setLeftChild(std::move(yNode));
  timesNode->setRightChild(std::move(zNode));

  auto plusNode = make_unique<PlusASTNode>();
  plusNode->setLeftChild(std::move(xNode));
  plusNode->setRightChild(std::move(timesNode));

  rightHeavyTree = make_unique<AST>(std::move(plusNode));
}
