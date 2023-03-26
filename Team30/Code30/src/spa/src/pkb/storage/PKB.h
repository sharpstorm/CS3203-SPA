#pragma once

#include <string>
#include <utility>

#include "EntityMappingProvider.h"
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

  EntityTable *procedureTable = new EntityTable();
  EntityRevTable *procedureRevTable = new EntityRevTable();
  ProcedureStorage *procedureStorage =
      new ProcedureStorage(procedureTable, procedureRevTable);

  StmtTable *stmtTable = new StmtTable();
  StmtRevTable *stmtRevTable = new StmtRevTable();
  StatementStorage *statementStorage =
      new StatementStorage(stmtTable, stmtRevTable);

  CallStmtTable *callStmtTable = new CallStmtTable();
  CallStmtRevTable *callStmtRevTable = new CallStmtRevTable();
  CallStmtStorage *callStmtStorage =
      new CallStmtStorage(callStmtTable, callStmtRevTable);

  StructureMappingProvider *structureProvider = new StructureMappingProvider(
      statementStorage, procedureStorage, callStmtStorage);
  EntityMappingProvider *entityMappingProvider = new EntityMappingProvider(
      variableStorage, constantStorage, procedureStorage);
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
  ParentStorage *parentStorage = new ParentStorage(parentTable, parentRevTable);
  ParentTStorage *parentTStorage =
      new ParentTStorage(parentTable, parentRevTable);

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
  CallsTStorage *callsTStorage = new CallsTStorage(callsTable, callsRevTable);

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
