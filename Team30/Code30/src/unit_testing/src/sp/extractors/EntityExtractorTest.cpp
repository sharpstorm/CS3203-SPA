#include <string>

#include "../../../../spa/src/common/ASTNode/ConstantASTNode.h"
#include "../../../../spa/src/common/ASTNode/statement/AssignNode.h"
#include "../../../../spa/src/common/ASTNode/statement/IfNode.h"
#include "../../../../spa/src/common/ASTNode/statement/PrintNode.h"
#include "../../../../spa/src/common/ASTNode/statement/WhileNode.h"
#include "../../../../spa/src/common/Types.h"
#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
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
    return (nodeValue == entityName) && (e_type == entityType);
  }
};

TEST_CASE("EntityExtractor PrintNode") {
  PrintNode node = new PrintNode();
  node.lineNumber = 1;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::Print);
}

TEST_CASE("EntityExtractor AssignNode") {
  AssignNode node = new AssignNode();
  node.lineNumber = 2;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::Assign);
}

TEST_CASE("EntityExtractor WhileNode") {
  WhileNode node = new WhileNode();
  node.lineNumber = 3;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::While);
}

TEST_CASE("EntityExtractor IfNode") {
  IfNode node = new IfNode();
  node.lineNumber = 4;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::If);
}

TEST_CASE("EntityExtractor ReadNode") {
  ReadNode node = new ReadNode();
  node.lineNumber = 5;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyStatement(node.lineNumber, StmtType::Read);
}

TEST_CASE("EntityExtractor VariableNode") {
  VariableASTNode node = new VariableASTNode();
  node.lineNumber = 6;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyEntity(node.getValue(), EntityType::Variable);
}

TEST_CASE("EntityExtractor ConstantNode") {
  ConstantASTNode node = new ConstantASTNode();
  node.lineNumber = 7;

  PkbWriterStubforEntity writer = new PkbWriterStubforEntity();
  EntityExtractor* extractor = new EntityExtractor(&writer);

  extractor->visit(node);

  REQUIRE(writer.verifyEntity(node.getValue(), EntityType::Constant);
}
