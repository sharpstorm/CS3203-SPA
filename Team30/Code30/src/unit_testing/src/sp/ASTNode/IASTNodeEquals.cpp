#include "catch.hpp"
#include "common/ASTNode/statement/PrintNode.h"
#include "common/ASTNode/statement/AssignNode.h"
#include "common/ASTNode/statement/ReadNode.h"
#include "common/ASTNode/statement/WhileNode.h"
#include "common/ASTNode/statement/IfNode.h"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/ConstantASTNode.h"
#include "common/ASTNode/math/math_operand/PlusASTNode.h"
#include "common/ASTNode/math/math_operand/MinusASTNode.h"
#include "common/ASTNode/math/math_operand/TimesASTNode.h"
#include "common/ASTNode/math/math_operand/ModASTNode.h"
#include "common/ASTNode/math/math_operand/DivASTNode.h"
#include "common/ASTNode/math/conditional_operand/AndASTNode.h"
#include "common/ASTNode/math/conditional_operand/EqualsASTNode.h"
#include "common/ASTNode/math/conditional_operand/GtASTNode.h"
#include "common/ASTNode/math/conditional_operand/GteASTNode.h"
#include "common/ASTNode/math/conditional_operand/LtASTNode.h"
#include "common/ASTNode/math/conditional_operand/LteASTNode.h"
#include "common/ASTNode/math/conditional_operand/NotASTNode.h"
#include "common/ASTNode/math/conditional_operand/NotEqualsASTNode.h"
#include "common/ASTNode/math/conditional_operand/OrASTNode.h"


using std::vector;

TEST_CASE("Test IASTNode equals - Print node") {
  PrintNode p;
  PrintNode p2;
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new PrintNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new PrintNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Assign node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new AssignNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new AssignNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Read node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new ReadNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new ReadNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - While node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new WhileNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new WhileNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - If node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new IfNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new IfNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode not equals") {
  PrintNode n1;
  PrintNode n2;
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new ReadNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new PrintNode());
  shared_ptr<IASTNode> node3 = shared_ptr<IASTNode>(new WhileNode());
  shared_ptr<IASTNode> node4 = shared_ptr<IASTNode>(new IfNode());
  shared_ptr<IASTNode> node5 = shared_ptr<IASTNode>(new AssignNode());
  REQUIRE(node1->isEquals(*node2.get()));
  REQUIRE(node2->isEquals(*node3.get()));
  REQUIRE(node3->isEquals(*node4.get()));
  REQUIRE(node4->isEquals(*node5.get()));
  REQUIRE(node5->isEquals(*node1.get()));
}

TEST_CASE("Test IASTNode Equals Variable ") {
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new VariableASTNode("a"));
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new VariableASTNode("a"));
  shared_ptr<IASTNode> node3 = shared_ptr<IASTNode>(new VariableASTNode("b"));
  REQUIRE(node1->isEquals(*node2.get()));
  REQUIRE(!node1->isEquals(*node3.get()));
}

TEST_CASE("Test IASTNode Equals Constant ") {
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new ConstantASTNode("1"));
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new ConstantASTNode("1"));
  shared_ptr<IASTNode> node3 = shared_ptr<IASTNode>(new ConstantASTNode("2"));
  REQUIRE(node1->isEquals(*node2.get()));
  REQUIRE(!node1->isEquals(*node3.get()));
}

TEST_CASE("Test IASTNode equals - Plus node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new PlusASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new PlusASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Minus node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new MinusASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new MinusASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Mod node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new ModASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new ModASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Times node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new TimesASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new TimesASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Div node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new DivASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new DivASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode operand not equals") {
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new PlusASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new MinusASTNode());
  shared_ptr<IASTNode> node3 = shared_ptr<IASTNode>(new TimesASTNode());
  shared_ptr<IASTNode> node4 = shared_ptr<IASTNode>(new DivASTNode());
  shared_ptr<IASTNode> node5 = shared_ptr<IASTNode>(new ModASTNode());
  REQUIRE(node1->isEquals(*node2.get()));
  REQUIRE(node2->isEquals(*node3.get()));
  REQUIRE(node3->isEquals(*node4.get()));
  REQUIRE(node4->isEquals(*node5.get()));
  REQUIRE(node5->isEquals(*node1.get()));
}

TEST_CASE("Test IASTNode equals - And node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new AndASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new AndASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Equals node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new EqualsASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new EqualsASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Gt node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new GtASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new GtASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Gte node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new GteASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new GteASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Lt node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new LtASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new LtASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Lte node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new LteASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new LteASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Not node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new NotASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new NotASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Not Equals node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new NotEqualsASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new NotEqualsASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}

TEST_CASE("Test IASTNode equals - Or node") {
  shared_ptr<IASTNode> node = shared_ptr<IASTNode>(new OrASTNode());
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new OrASTNode());
  REQUIRE(node->isEquals(*node2.get()));
}
