#include <string>

#include "../../../../../lib/catch.hpp"
#include "../../../../spa/src/common/ASTNode/ConstantASTNode.h"
#include "../../../../spa/src/common/ASTNode/statement/AssignNode.h"
#include "../../../../spa/src/common/ASTNode/statement/IfNode.h"
#include "../../../../spa/src/common/ASTNode/statement/PrintNode.h"
#include "../../../../spa/src/common/ASTNode/statement/WhileNode.h"
#include "../../../../spa/src/common/Types.h"
#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/EntityExtractor.h"
class PkbWriterStubforEntity : public PkbWriter {
 public:
  StmtType s_type;
  int lineNum;
  EntityType e_Type;
  string nodeValue;

  PkbWriterStubforEntity(PKB* pkb) : PkbWriter(pkb) {}

  void addStatement(int lineNumber, StmtType stmtType) final {
    s_type = stmtType;
    lineNum = lineNumber;
  }

  void addSymbol(string entityName, EntityType entityType) final {
    nodeValue = entityName;
    e_Type = entityType;
  }

  int verifyStatement(int lineNumber, StmtType stmtType) {
    return (lineNum == lineNumber) && (s_type == stmtType);
  }

  int verifyEntity(string entityName, EntityType entityType) {
    return (nodeValue == entityName) && (e_Type == entityType);
  }
};

TEST_CASE("EntityExtractor PrintNode") {
  PrintNode node;
  node.lineNumber = 1;

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::Print));
}

TEST_CASE("EntityExtractor AssignNode") {
  AssignNode node;
  node.lineNumber = 2;

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::Assign));
}

TEST_CASE("EntityExtractor WhileNode") {
  WhileNode node;
  node.lineNumber = 3;

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::While));
}

TEST_CASE("EntityExtractor IfNode") {
  IfNode node;
  node.lineNumber = 4;

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::If));
}

TEST_CASE("EntityExtractor ReadNode") {
  ReadNode node;
  node.lineNumber = 5;

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::Read));
}

TEST_CASE("EntityExtractor VariableNode") {
  VariableASTNode node("test");

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyEntity(node.getValue(), EntityType::Variable));
}

TEST_CASE("EntityExtractor ConstantNode") {
  ConstantASTNode node("test");

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visit(node);

  REQUIRE(writer.verifyEntity(node.getValue(), EntityType::Constant));
}
