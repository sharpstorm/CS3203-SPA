#include "catch.hpp"
#include "sp/ast/statement/PrintNode.h"
#include "sp/ast/statement/AssignNode.h"
#include "sp/ast/statement/ReadNode.h"
#include "sp/ast/statement/WhileNode.h"
#include "sp/ast/statement/IfNode.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/expression_operand/MinusASTNode.h"
#include "sp/ast/expression_operand/TimesASTNode.h"
#include "sp/ast/expression_operand/ModASTNode.h"
#include "sp/ast/expression_operand/DivASTNode.h"
#include "sp/ast/conditional_operand/AndASTNode.h"
#include "sp/ast/conditional_operand/EqualsASTNode.h"
#include "sp/ast/conditional_operand/GtASTNode.h"
#include "sp/ast/conditional_operand/GteASTNode.h"
#include "sp/ast/conditional_operand/LtASTNode.h"
#include "sp/ast/conditional_operand/LteASTNode.h"
#include "sp/ast/conditional_operand/NotASTNode.h"
#include "sp/ast/conditional_operand/NotEqualsASTNode.h"
#include "sp/ast/conditional_operand/OrASTNode.h"

using std::vector, std::make_shared;

TEST_CASE("Test IASTNode equals - Print node") {
  shared_ptr<IASTNode> node = make_shared<PrintNode>(1);
  shared_ptr<IASTNode> node2 = make_shared<PrintNode>(1);
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Assign node") {
  shared_ptr<IASTNode> node = make_shared<AssignNode>(1);
  shared_ptr<IASTNode> node2 = make_shared<AssignNode>(1);
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Read node") {
  shared_ptr<IASTNode> node = make_shared<ReadNode>(1);
  shared_ptr<IASTNode> node2 = make_shared<ReadNode>(1);
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - While node") {
  shared_ptr<IASTNode> node = make_shared<WhileNode>(1);
  shared_ptr<IASTNode> node2 = make_shared<WhileNode>(1);
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - If node") {
  shared_ptr<IASTNode> node = make_shared<IfNode>(1);
  shared_ptr<IASTNode> node2 = make_shared<IfNode>(1);
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode not equals") {
  shared_ptr<IASTNode> node1 = make_shared<ReadNode>(1);
  shared_ptr<IASTNode> node2 = make_shared<PrintNode>(1);
  shared_ptr<IASTNode> node3 = make_shared<WhileNode>(1);
  shared_ptr<IASTNode> node4 = make_shared<IfNode>(1);
  shared_ptr<IASTNode> node5 = make_shared<AssignNode>(1);
  REQUIRE(!node1->isEquals(node2.get()));
  REQUIRE(!node2->isEquals(node3.get()));
  REQUIRE(!node3->isEquals(node4.get()));
  REQUIRE(!node4->isEquals(node5.get()));
  REQUIRE(!node5->isEquals(node1.get()));
}

TEST_CASE("Test IASTNode Equals Variable ") {
  shared_ptr<IASTNode> node1 = make_shared<VariableASTNode>("a");
  shared_ptr<IASTNode> node2 = make_shared<VariableASTNode>("a");
  shared_ptr<IASTNode> node3 = make_shared<VariableASTNode>("b");
  REQUIRE(node1->isEquals(node2.get()));
  REQUIRE(!node1->isEquals(node3.get()));
}

TEST_CASE("Test IASTNode Equals Constant ") {
  shared_ptr<IASTNode> node1 = make_shared<ConstantASTNode>("1");
  shared_ptr<IASTNode> node2 = make_shared<ConstantASTNode>("1");
  shared_ptr<IASTNode> node3 = make_shared<ConstantASTNode>("2");
  REQUIRE(node1->isEquals(node2.get()));
  REQUIRE(!node1->isEquals(node3.get()));
}

TEST_CASE("Test IASTNode equals - Plus node") {
  shared_ptr<IASTNode> node = make_shared<PlusASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<PlusASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Minus node") {
  shared_ptr<IASTNode> node = make_shared<MinusASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<MinusASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Mod node") {
  shared_ptr<IASTNode> node = make_shared<ModASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<ModASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Times node") {
  shared_ptr<IASTNode> node = make_shared<TimesASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<TimesASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Div node") {
  shared_ptr<IASTNode> node = make_shared<DivASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<DivASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode operand not equals") {
  shared_ptr<IASTNode> node1 = make_shared<PlusASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<MinusASTNode>();
  shared_ptr<IASTNode> node3 = make_shared<TimesASTNode>();
  shared_ptr<IASTNode> node4 = make_shared<DivASTNode>();
  shared_ptr<IASTNode> node5 = make_shared<ModASTNode>();
  REQUIRE(!node1->isEquals(node2.get()));
  REQUIRE(!node2->isEquals(node3.get()));
  REQUIRE(!node3->isEquals(node4.get()));
  REQUIRE(!node4->isEquals(node5.get()));
  REQUIRE(!node5->isEquals(node1.get()));
}

TEST_CASE("Test IASTNode equals - And node") {
  shared_ptr<IASTNode> node = make_shared<AndASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<AndASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Equals node") {
  shared_ptr<IASTNode> node = make_shared<EqualsASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<EqualsASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Gt node") {
  shared_ptr<IASTNode> node = make_shared<GtASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<GtASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Gte node") {
  shared_ptr<IASTNode> node = make_shared<GteASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<GteASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Lt node") {
  shared_ptr<IASTNode> node = make_shared<LtASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<LtASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Lte node") {
  shared_ptr<IASTNode> node = make_shared<LteASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<LteASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Not node") {
  shared_ptr<IASTNode> node = make_shared<NotASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<NotASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Not Equals node") {
  shared_ptr<IASTNode> node = make_shared<NotEqualsASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<NotEqualsASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}

TEST_CASE("Test IASTNode equals - Or node") {
  shared_ptr<IASTNode> node = make_shared<OrASTNode>();
  shared_ptr<IASTNode> node2 = make_shared<OrASTNode>();
  REQUIRE(node->isEquals(node2.get()));
}
