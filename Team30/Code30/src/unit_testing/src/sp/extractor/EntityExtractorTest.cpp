#include <string>

#include "../../../../../lib/catch.hpp"
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

  EntityIdx addVariable(string entityName) final {
    nodeValue = entityName;
    e_Type = EntityType::Variable;
    return 1;
  }

  EntityIdx addConstant(string entityName) final {
    nodeValue = entityName;
    e_Type = EntityType::Constant;
    return 1;
  }

  int verifyStatement(int lineNumber, StmtType stmtType) {
    return (lineNum == lineNumber) && (s_type == stmtType);
  }

  int verifyEntity(string entityName, EntityType entityType) {
    return (nodeValue == entityName) && (e_Type == entityType);
  }
};

TEST_CASE("EntityExtractor PrintNode") {
  PrintNode node(1);

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitPrint(&node);

  REQUIRE(writer.verifyStatement(node.getLineNumber(), StmtType::Print));
}

TEST_CASE("EntityExtractor AssignNode") {
  AssignNode node(2);

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitAssign(&node);

  REQUIRE(writer.verifyStatement(node.getLineNumber(), StmtType::Assign));
}

TEST_CASE("EntityExtractor WhileNode") {
  WhileNode node(3);

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitWhile(&node);

  REQUIRE(writer.verifyStatement(node.getLineNumber(), StmtType::While));
}

TEST_CASE("EntityExtractor IfNode") {
  IfNode node(4);

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitIf(&node);

  REQUIRE(writer.verifyStatement(node.getLineNumber(), StmtType::If));
}

TEST_CASE("EntityExtractor ReadNode") {
  ReadNode node(5);

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitRead(&node);

  REQUIRE(writer.verifyStatement(node.getLineNumber(), StmtType::Read));
}

TEST_CASE("EntityExtractor CallNode") {
  ProcedureNode pNode("main");
  CallNode node(6, "sub");
  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);
  extractor.visitProcedure(&pNode);
  extractor.visitCall(&node);
  REQUIRE(writer.verifyStatement(node.getLineNumber(), StmtType::Call));
}

TEST_CASE("EntityExtractor VariableNode") {
  VariableASTNode node("test");

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitVariable(&node);

  REQUIRE(writer.verifyEntity(node.getValue(), EntityType::Variable));
}

TEST_CASE("EntityExtractor ConstantNode") {
  ConstantASTNode node("test");

  PKB pkb;
  PkbWriterStubforEntity writer(&pkb);
  EntityExtractor extractor(&writer);

  extractor.visitConstant(&node);

  REQUIRE(writer.verifyEntity(node.getValue(), EntityType::Constant));
}
