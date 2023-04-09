#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique, std::make_shared;

TEST_CASE("CFGs write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());

  auto CFGRoot = make_shared<CFG>(1);
  CFGRoot->addLink(0, 1);
  CFGRoot->addLink(1, 2);
  CFGRoot->addLink(2, 3);
  CFGRoot->addLink(3, CFG_END_NODE);

  writer.addStatement(1, StmtType::Assign);
  writer.addProcedure("main", 1, 3);
  writer.addCFGs("main", CFGRoot);
  auto result = queryHandler.queryCFGs({StmtType::Assign, 1});

  REQUIRE(result.size() == 1);
  REQUIRE(result.at(0)->getNodeCount() == 4);
}
