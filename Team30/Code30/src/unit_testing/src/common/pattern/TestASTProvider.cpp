#include "TestASTProvider.h"
#include "sp/ast/ASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/TimesASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/AST.h"

#include <memory>

using std::make_shared, std::make_unique;

TestASTProvider::TestASTProvider() {
  buildBalancedTree();
  buildLeftHeavyTree();
  buildRightHeavyTree();
}

void TestASTProvider::buildBalancedTree() {
  // x * y + x * z;
  ASTNodePtr xNode1 = make_shared<VariableASTNode>("x");
  ASTNodePtr xNode2 = make_shared<VariableASTNode>("x");
  ASTNodePtr yNode = make_shared<VariableASTNode>("y");
  ASTNodePtr zNode = make_shared<VariableASTNode>("z");

  auto timesNode1 = make_shared<TimesASTNode>();
  timesNode1->setLeftChild(xNode1);
  timesNode1->setRightChild(yNode);

  auto timesNode2 = make_shared<TimesASTNode>();
  timesNode2->setLeftChild(xNode2);
  timesNode2->setRightChild(zNode);

  auto plusNode = make_shared<PlusASTNode>();
  plusNode->setLeftChild(timesNode1);
  plusNode->setRightChild(timesNode2);

  balancedTree = make_unique<AST>(plusNode);
}

void TestASTProvider::buildLeftHeavyTree() {
  // x * y + z;
  ASTNodePtr xNode = make_shared<VariableASTNode>("x");
  ASTNodePtr yNode = make_shared<VariableASTNode>("y");
  ASTNodePtr zNode = make_shared<VariableASTNode>("z");

  auto timesNode = make_shared<TimesASTNode>();
  timesNode->setLeftChild(xNode);
  timesNode->setRightChild(yNode);

  auto plusNode = make_shared<PlusASTNode>();
  plusNode->setLeftChild(timesNode);
  plusNode->setRightChild(zNode);

  leftHeavyTree = make_unique<AST>(plusNode);
}

void TestASTProvider::buildRightHeavyTree() {
  ASTNodePtr xNode = make_shared<VariableASTNode>("x");
  ASTNodePtr yNode = make_shared<VariableASTNode>("y");
  ASTNodePtr zNode = make_shared<VariableASTNode>("z");

  auto timesNode = make_shared<TimesASTNode>();
  timesNode->setLeftChild(yNode);
  timesNode->setRightChild(zNode);

  auto plusNode = make_shared<PlusASTNode>();
  plusNode->setLeftChild(xNode);
  plusNode->setRightChild(timesNode);

  rightHeavyTree = make_unique<AST>(plusNode);
}
