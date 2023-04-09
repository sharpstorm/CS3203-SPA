#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/ProcedureAndCallsStorage.h"
#include "pkb/storage/StorageTypes.h"

TEST_CASE("ProcedureAndCallsStorage addProcedure") {
  auto procedureStmtTable = make_unique<ProcedureStmtTable>();
  auto callDeclarationTable = make_unique<CallDeclarationTable>();
  auto procedureValues = make_unique<ProcedureValues>();
  ProcedureAndCallsStorage store(procedureStmtTable.get(),
                                 callDeclarationTable.get(),
                                 procedureValues.get());

  store.addProcedure(1, 3, "procA");
  store.addProcedure(4, 4, "procB");

  REQUIRE(store.getProcedures() == EntityValueSet({"procA", "procB"}));
  REQUIRE(*procedureValues == EntityValueSet({"procA", "procB"}));
  REQUIRE(store.getProcedureForLine(1) == "procA");
  REQUIRE(store.getProcedureForLine(2) == "procA");
  REQUIRE(store.getProcedureForLine(3) == "procA");
  REQUIRE(store.getProcedureForLine(4) == "procB");
  REQUIRE(store.getProcedureForLine(5) == "");

  REQUIRE(store.procedureExists("procB") == true);
  REQUIRE(store.procedureExists("procC") == false);
}

TEST_CASE("ProcedureAndCallsStorage addCallDeclaration") {
  auto procedureStmtTable = make_unique<ProcedureStmtTable>();
  auto callDeclarationTable = make_unique<CallDeclarationTable>();
  auto procedureValues = make_unique<ProcedureValues>();
  ProcedureAndCallsStorage store(procedureStmtTable.get(),
                                 callDeclarationTable.get(),
                                 procedureValues.get());

  store.addProcedure(1, 3, "procA");
  store.addProcedure(4, 4, "procB");
  store.addProcedure(5, 6, "procC");
  store.addCallDeclaration(3, "procB");
  store.addCallDeclaration(4, "procC");

  REQUIRE(store.getCalledDeclaration(3) == "procB");
  REQUIRE(store.getCalledDeclaration(4) == "procC");
  REQUIRE(callDeclarationTable->get(3) == "procB");
  REQUIRE(callDeclarationTable->get(4) == "procC");

  REQUIRE(store.hasUndefinedProcedures() == false);
}

TEST_CASE("ProcedureAndCallsStorage hasUndefinedProcedures") {
  auto procedureStmtTable = make_unique<ProcedureStmtTable>();
  auto callDeclarationTable = make_unique<CallDeclarationTable>();
  auto procedureValues = make_unique<ProcedureValues>();
  ProcedureAndCallsStorage store(procedureStmtTable.get(),
                                 callDeclarationTable.get(),
                                 procedureValues.get());

  store.addProcedure(1, 3, "procA");
  store.addCallDeclaration(3, "procB");

  REQUIRE(store.hasUndefinedProcedures() == true);
}
