#include <memory>
#include <string>

#include "../../../../../lib/catch.hpp"
#include "../../../../spa/src/common/ASTNode/VariableASTNode.h"
#include "../../../../spa/src/common/ASTNode/statement/AssignNode.h"
#include "../../../../spa/src/common/ASTNode/statement/PrintNode.h"
#include "../../../../spa/src/common/ASTNode/statement/ReadNode.h"
#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/ModifiesExtractor.h"

class PkbWriterStubforModifies : public PkbWriter {
 public:
  int num;
  std::string value;

  PkbWriterStubforModifies(PKB* pkb) : PkbWriter(pkb) {}

  void addModifies(int stmtNum, std::string variable) final {
    num = stmtNum;
    value = variable;
  }

  int verifyEntry(int stmtNum, std::string variable) {
    return (num == stmtNum) && (variable == value);
  }
};

TEST_CASE("ModifiesExtractor AssignNode") {
  shared_ptr<AssignNode> node = shared_ptr<AssignNode>(new AssignNode());

  shared_ptr<VariableASTNode> child =
      shared_ptr<VariableASTNode>(new VariableASTNode("TEST"));

  node->lineNumber = 2;
  node->addChild(child);

  PKB* pkb = new PKB();
  PkbWriterStubforModifies* writer = new PkbWriterStubforModifies(pkb);
  ModifiesExtractor* extractor = new ModifiesExtractor(writer);

  extractor->visit(*node);

  REQUIRE(writer->verifyEntry(node->lineNumber, "TEST"));
}

TEST_CASE("ModifiesExtractor ReadNode") {
  shared_ptr<ReadNode> node = shared_ptr<ReadNode>(new ReadNode());

  shared_ptr<VariableASTNode> child =
      shared_ptr<VariableASTNode>(new VariableASTNode("TEST"));

  node->lineNumber = 2;
  node->addChild(child);

  PKB* pkb = new PKB();
  PkbWriterStubforModifies* writer = new PkbWriterStubforModifies(pkb);
  ModifiesExtractor* extractor = new ModifiesExtractor(writer);

  extractor->visit(*node);

  REQUIRE(writer->verifyEntry(node->lineNumber, "TEST"));
}