#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "DesignEntitiesQueryHandler.h"
#include "PkbEntEntQueryInvoker.h"
#include "PkbStmtEntQueryInvoker.h"
#include "PkbStmtStmtQueryInvoker.h"
#include "pkb/queryHandlers/interfaces/IAssignsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/ICFGsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/ICallsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IIfPatternQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IModifiesQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IParentQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IParentTQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IPkbQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IUsesQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IWhilePatternQueryHandler.h"
#include "pkb/storage/PKB.h"

using std::string;
using std::unique_ptr, std::shared_ptr;
using std::unordered_set, std::vector;

class PkbQueryHandler : public IPkbQueryHandler {
 public:
  explicit PkbQueryHandler(PKB *pkb);

  QueryResultPtr<StmtValue, StmtValue> queryFollows(StmtRef,
                                                    StmtRef) const override;
  QueryResultPtr<StmtValue, StmtValue> queryFollowsStar(StmtRef,
                                                        StmtRef) const override;
  QueryResultPtr<StmtValue, StmtValue> queryParent(StmtRef,
                                                   StmtRef) const override;
  QueryResultPtr<StmtValue, StmtValue> queryParentStar(StmtRef,
                                                       StmtRef) const override;
  QueryResultPtr<StmtValue, EntityValue> queryUses(StmtRef,
                                                   EntityRef) const override;
  QueryResultPtr<EntityValue, EntityValue> queryUses(EntityRef,
                                                     EntityRef) const override;
  QueryResultPtr<StmtValue, EntityValue> queryModifies(
      StmtRef, EntityRef) const override;
  QueryResultPtr<EntityValue, EntityValue> queryModifies(
      EntityRef, EntityRef) const override;
  QueryResultPtr<StmtValue, PatternTrie *> queryAssigns(StmtRef) const override;
  QueryResultPtr<EntityValue, EntityValue> queryCalls(EntityRef,
                                                      EntityRef) const override;
  QueryResultPtr<EntityValue, EntityValue> queryCallsStar(
      EntityRef, EntityRef) const override;
  QueryResultPtr<StmtValue, EntityValue> queryIfPattern(
      StmtRef, EntityRef) const override;
  QueryResultPtr<StmtValue, EntityValue> queryWhilePattern(
      StmtRef, EntityRef) const override;
  EntityValueSet getSymbolsOfType(EntityType) const override;
  StmtValueSet getStatementsOfType(StmtType) const override;
  StmtType getStatementType(StmtValue) const override;
  EntityValue getVariableByIndex(EntityIdx) const override;
  EntityValue getConstantByIndex(EntityIdx) const override;
  EntityIdx getIndexOfVariable(EntityValue) const override;
  EntityIdx getIndexOfConstant(EntityValue) const override;
  vector<CFG *> queryCFGs(StmtRef) const override;
  bool isStatementOfType(StmtType, StmtValue) const override;
  bool isSymbolOfType(EntityType, EntityValue) const override;
  EntityValue getCalledDeclaration(StmtValue) const override;
  EntityValue getReadDeclarations(StmtValue) const override;
  EntityValue getPrintDeclarations(StmtValue) const override;

 private:
  unique_ptr<PkbStmtStmtQueryInvoker> stmtStmtQueryInvoker;
  unique_ptr<PkbStmtEntQueryInvoker> stmtEntQueryInvoker;
  unique_ptr<PkbEntEntQueryInvoker> entEntQueryInvoker;

  unique_ptr<IDesignEntitiesQueryHandler> designEntityHandler;
  unique_ptr<IFollowsQueryHandler> followsHandler;
  unique_ptr<IFollowsQueryHandler> followsTHandler;
  unique_ptr<IParentQueryHandler> parentHandler;
  unique_ptr<IParentTQueryHandler> parentTHandler;
  unique_ptr<IModifiesQueryHandler> modifiesHandler;
  unique_ptr<IUsesQueryHandler> usesHandler;
  unique_ptr<ICallsQueryHandler> callsHandler;
  unique_ptr<ICallsQueryHandler> callsTHandler;

  unique_ptr<IIfPatternQueryHandler> ifPatternHandler;
  unique_ptr<IWhilePatternQueryHandler> whilePatternHandler;
  unique_ptr<IAssignsQueryHandler> assignHandler;
  unique_ptr<ICFGsQueryHandler> cfgsHandler;
};
