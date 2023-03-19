#include <memory>
#include <unordered_set>

#include "EntityMappingProviderStub.h"
#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/CFGsQueryHandler.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

static unique_ptr<EntityMappingProviderStub> setUpEntityMappingProvider() {
  auto provider = make_unique<EntityMappingProviderStub>();
  provider->procedureTable.set(1, "main");
  provider->procedureTable.set(2, "main");
  provider->procedureTable.set(3, "main");
  provider->variableTable.set(1, "x");
  provider->variableTable.set(2, "y");
  provider->variableTable.set(3, "z");
  provider->allVariables = {"x", "y", "z"};
  provider->allProcedures = {"main"};
  return provider;
}

static unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.set(1, StmtType::Assign);
  provider->stmtNumToType.set(2, StmtType::Assign);
  provider->stmtNumToType.set(3, StmtType::Assign);

  provider->stmtTypeToNum.set(StmtType::Assign, 1);
  provider->stmtTypeToNum.set(StmtType::Assign, 2);
  provider->stmtTypeToNum.set(StmtType::Assign, 3);

  provider->stmtNumToProcedure.set(1, "main");
  provider->stmtNumToProcedure.set(2, "main");
  provider->stmtNumToProcedure.set(3, "main");
  provider->allStmts = {1, 2, 3};
  return provider;
}

struct CFGSTestInit {
  unique_ptr<CFGStorage> store;
  unique_ptr<EntityMappingProviderStub> entityProvider;
  unique_ptr<StructureMappingProviderStub> structureProvider;

  CFGsQueryHandler handler;

  CFGSTestInit()
      : store(make_unique<CFGStorage>()),
        entityProvider(setUpEntityMappingProvider()),
        structureProvider(setUpStructureMappingProvider()),
        handler(CFGsQueryHandler(store.get(), entityProvider.get(),
                                 structureProvider.get())) {}
};

// assign stmt
TEST_CASE("CFGsQueryHandler next/next* query(stmtNum)") {
  auto test = CFGSTestInit();

  auto CFGRoot = make_shared<CFG>(1);
  CFGRoot->addLink(0, 1);
  CFGRoot->addLink(1, 2);
  CFGRoot->addLink(2, 3);
  CFGRoot->addLink(3, CFG_END_NODE);
  test.store->set("main", CFGRoot);

  auto result = test.handler.queryCFGs({StmtType::Assign, 1});
  REQUIRE(result.size() == 1);
  REQUIRE(result.at(0)->getNodeCount() == 4);
}
