#include <memory>
#include <string>

#include "../../../../../lib/catch.hpp"
#include "../../../../spa/src/common/ASTNode/VariableASTNode.h"
#include "../../../../spa/src/common/ASTNode/statement/AssignNode.h"
#include "../../../../spa/src/common/ASTNode/statement/PrintNode.h"
#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/ModifiesExtractor.h"
class PkbWriterStubforModifies : public PkbWriter {
 public:
  int num;
  string value;

  PkbWriterStubforModifies(PKB* pkb) : PkbWriter(pkb) {}

  void addModifies(int stmtNum, string variable) final {
    num = stmtNum;
    value = variable;
  }

  int verifyEntry(int stmtNum, string variable) {
    return (num == stmtNum) && (variable == value);
  }
};

//TEST_CASE("ModifiesExtractor AssignNode") {
//  AssignNode* node = new AssignNode();
//
//  shared_ptr<StatementASTNode> child =
//      shared_ptr<StatementASTNode>(new PrintNode());
//
//  node->lineNumber = 3;
//  node->addChild(child);
//
//  string value = std::dynamic_pointer_cast<VariableASTNode>(node->getChildren()[0])->getValue();
//
//  PKB* pkb = new PKB();
//  PkbWriterStubforModifies* writer = new PkbWriterStubforModifies(pkb);
//  ModifiesExtractor* extractor = new ModifiesExtractor(writer);
//
//  extractor->visit(*node);
//
//  REQUIRE(writer->verifyEntry(node->lineNumber, value));
//}
//
//TEST_CASE("ModifiesExtractor ReadNode") {
//  ReadNode* node = new ReadNode();
//
//  shared_ptr<StatementASTNode> child =
//      shared_ptr<StatementASTNode>(new PrintNode());
//
//  node->lineNumber = 4;
//  node->addChild(child);
//
//  string value =
//      std::dynamic_pointer_cast<VariableASTNode>(node->getChildren()[0])
//          ->getValue();
//
//  PKB* pkb = new PKB();
//  PkbWriterStubforModifies* writer = new PkbWriterStubforModifies(pkb);
//  ModifiesExtractor* extractor = new ModifiesExtractor(writer);
//
//  extractor->visit(*node);
//
//  REQUIRE(writer->verifyEntry(node->lineNumber, value));
//}
