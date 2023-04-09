#pragma once

#include <memory>
#include <vector>

#include "common/pattern/PatternTrie.h"

using std::unique_ptr;
using std::vector;

class IPkbQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
  virtual QueryResultPtr<StmtValue, StmtValue> queryFollows(StmtRef,
                                                            StmtRef) const = 0;
  virtual QueryResultPtr<StmtValue, StmtValue> queryFollowsStar(
      StmtRef, StmtRef) const = 0;
  virtual QueryResultPtr<StmtValue, StmtValue> queryParent(StmtRef,
                                                           StmtRef) const = 0;
  virtual QueryResultPtr<StmtValue, StmtValue> queryParentStar(
      StmtRef, StmtRef) const = 0;
  virtual QueryResultPtr<StmtValue, EntityValue> queryUses(StmtRef,
                                                           EntityRef) const = 0;
  virtual QueryResultPtr<EntityValue, EntityValue> queryUses(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<StmtValue, EntityValue> queryModifies(
      StmtRef, EntityRef) const = 0;
  virtual QueryResultPtr<EntityValue, EntityValue> queryModifies(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<StmtValue, PatternTrie *> queryAssigns(
      StmtRef) const = 0;
  virtual QueryResultPtr<EntityValue, EntityValue> queryCalls(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<EntityValue, EntityValue> queryCallsStar(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<StmtValue, EntityValue> queryIfPattern(
      StmtRef, EntityRef) const = 0;
  virtual QueryResultPtr<StmtValue, EntityValue> queryWhilePattern(
      StmtRef, EntityRef) const = 0;
  virtual EntityValueSet getSymbolsOfType(EntityType) const = 0;
  virtual StmtValueSet getStatementsOfType(StmtType) const = 0;
  virtual StmtType getStatementType(StmtValue) const = 0;
  virtual EntityValue getVariableByIndex(EntityIdx) const = 0;
  virtual EntityValue getConstantByIndex(EntityIdx) const = 0;
  virtual EntityIdx getIndexOfVariable(EntityValue) const = 0;
  virtual EntityIdx getIndexOfConstant(EntityValue) const = 0;
  virtual vector<CFG *> queryCFGs(StmtRef) const = 0;
  virtual bool isStatementOfType(StmtType, StmtValue) const = 0;
  virtual bool isSymbolOfType(EntityType, EntityValue) const = 0;
  virtual EntityValue getCalledDeclaration(StmtValue) const = 0;
  virtual EntityValue getReadDeclarations(StmtValue) const = 0;
  virtual EntityValue getPrintDeclarations(StmtValue) const = 0;
};
