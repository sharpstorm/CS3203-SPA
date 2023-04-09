#pragma once

#include <utility>

#include "EntityMappingProvider.h"
#include "ProcedureAndCallsStorage.h"
#include "StorageTypes.h"
#include "StructureMappingProvider.h"
#include "common/Types.h"
#include "pkb/predicates/EntityPredicateFactory.h"
#include "pkb/predicates/StmtPredicateFactory.h"

class PKB {
 public:
  PKB();
  ~PKB();

  VarTable *variableTable = new VarTable();
  VarRevTable *variableRevTable = new VarRevTable();
  EntityValueSet *variableValues = new EntityValueSet;
  VariableStorage *variableStorage =
      new VariableStorage(variableTable, variableRevTable, variableValues);

  ConstTable *constantTable = new ConstTable();
  ConstRevTable *constantRevTable = new ConstRevTable();
  EntityValueSet *constantValues = new EntityValueSet;

  ConstantStorage *constantStorage =
      new ConstantStorage(constantTable, constantRevTable, constantValues);

  ProcedureStmtTable *procedureStmtTable = new ProcedureStmtTable();
  ProcedureValues *procedureValues = new ProcedureValues();
  CallDeclarationTable *callDeclarationTable = new CallDeclarationTable();
  ProcedureAndCallsStorage *procAndCallsStorage = new ProcedureAndCallsStorage(
      procedureStmtTable, callDeclarationTable, procedureValues);

  StmtTable *stmtTable = new StmtTable();
  StmtRevTable *stmtRevTable = new StmtRevTable();
  StmtValueSet *stmtValues = new StmtValueSet();
  StatementStorage *statementStorage =
      new StatementStorage(stmtTable, stmtRevTable, stmtValues);

  StructureMappingProvider *structureProvider =
      new StructureMappingProvider(statementStorage, procAndCallsStorage);
  EntityMappingProvider *entityMappingProvider = new EntityMappingProvider(
      variableStorage, constantStorage, procAndCallsStorage);
  StmtPredicateFactory *stmtPredicateFactory =
      new StmtPredicateFactory(structureProvider);
  EntityPredicateFactory *entityPredicateFactory = new EntityPredicateFactory();

  FollowsTable *followsTable = new FollowsTable();
  FollowsRevTable *followsRevTable = new FollowsRevTable();
  FollowsStorage *followsStorage =
      new FollowsStorage(followsTable, followsRevTable);
  FollowsTStorage *followsTStorage =
      new FollowsTStorage(followsTable, followsRevTable);

  ParentTable *parentTable = new ParentTable();
  ParentRevTable *parentRevTable = new ParentRevTable();
  ParentTTable *parentTTable = new ParentTTable();
  ParentTRevTable *parentTRevTable = new ParentTRevTable();
  ParentStorage *parentStorage = new ParentStorage(parentTable, parentRevTable);
  ParentTStorage *parentTStorage =
      new ParentTStorage(parentTTable, parentTRevTable, followsTable);

  ModifiesTable *modifiesTable = new ModifiesTable();
  ModifiesRevTable *modifiesRevTable = new ModifiesRevTable();
  ModifiesStorage *modifiesStorage =
      new ModifiesStorage(modifiesTable, modifiesRevTable);

  ModifiesPTable *modifiesPTable = new ModifiesPTable();
  ModifiesPRevTable *modifiesPRevTable = new ModifiesPRevTable();
  ModifiesPStorage *modifiesPStorage =
      new ModifiesPStorage(modifiesPTable, modifiesPRevTable);

  UsesTable *usesTable = new UsesTable();
  UsesRevTable *usesRevTable = new UsesRevTable();
  UsesStorage *usesStorage = new UsesStorage(usesTable, usesRevTable);

  UsesPTable *usesPTable = new UsesPTable();
  UsesPRevTable *usesPRevTable = new UsesPRevTable();
  UsesPStorage *usesPStorage = new UsesPStorage(usesPTable, usesPRevTable);

  CallsTable *callsTable = new CallsTable();
  CallsRevTable *callsRevTable = new CallsRevTable();
  CallsStorage *callsStorage = new CallsStorage(callsTable, callsRevTable);
  CallsTTable *callsTTable = new CallsTTable();
  CallsTRevTable *callsTRevTable = new CallsTRevTable();
  CallsTStorage *callsTStorage = new CallsTStorage(callsTTable, callsTRevTable);

  IfPatternTable *ifPatternTable = new IfPatternTable();
  IfPatternRevTable *ifPatternRevTable = new IfPatternRevTable();
  IfPatternStorage *ifPatternStorage =
      new IfPatternStorage(ifPatternTable, ifPatternRevTable);

  WhilePatternTable *whilePatternTable = new WhilePatternTable();
  WhilePatternRevTable *whilePatternRevTable = new WhilePatternRevTable();
  WhilePatternStorage *whilePatternStorage =
      new WhilePatternStorage(whilePatternTable, whilePatternRevTable);

  AssignStorage *assignStorage = new AssignStorage();
  CFGStorage *cfgStorage = new CFGStorage();
};
