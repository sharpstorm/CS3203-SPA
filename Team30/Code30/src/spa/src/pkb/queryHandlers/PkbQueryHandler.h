#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "DesignEntitiesQueryHandler.h"
#include "FollowsQueryHandler.h"
#include "ModifiesQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"
#include "pkb/queryHandlers/interfaces/IAssignsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/ICFGsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/ICallsQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IIfPatternQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IParentQueryHandler.h"
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

  QueryResultPtr<int, int> queryFollows(StmtRef,
                                                 StmtRef) const override;
  QueryResultPtr<int, int> queryFollowsStar(StmtRef,
                                                     StmtRef) const override;
  QueryResultPtr<int, int> queryParent(StmtRef,
                                                StmtRef) const override;
  QueryResultPtr<int, int> queryParentStar(StmtRef,
                                                    StmtRef) const override;
  QueryResultPtr<int, string> queryUses(StmtRef,
                                                 EntityRef) const override;
  QueryResultPtr<string, string> queryUses(EntityRef,
                                                    EntityRef) const override;
  QueryResultPtr<int, string> queryModifies(StmtRef,
                                                     EntityRef) const override;
  QueryResultPtr<string, string> queryModifies(
      EntityRef, EntityRef) const override;
  QueryResultPtr<int, PatternTrie *> queryAssigns(
      StmtRef) const override;
  QueryResultPtr<string, string> queryCalls(EntityRef,
                                                     EntityRef) const override;
  QueryResultPtr<string, string> queryCallsStar(
      EntityRef, EntityRef) const override;
  QueryResultPtr<int, string> queryIfPattern(StmtRef,
                                                      EntityRef) const override;
  QueryResultPtr<int, string> queryWhilePattern(
      StmtRef, EntityRef) const override;
  unordered_set<string> getSymbolsOfType(EntityType) const override;
  unordered_set<int> getStatementsOfType(StmtType) const override;
  StmtType getStatementType(int) const override;
  string getVariableByIndex(int) const override;
  string getConstantByIndex(int) const override;
  unordered_set<int> getIndexOfVariable(string) const override;
  unordered_set<int> getIndexOfConstant(string) const override;
  vector<CFG *> queryCFGs(StmtRef) const override;
  bool isStatementOfType(StmtType, int) const override;
  bool isSymbolOfType(EntityType, string) const override;
  string getCalledDeclaration(int) const override;
  string getReadDeclarations(int) const override;
  string getPrintDeclarations(int) const override;

 private:
  unique_ptr<PkbStmtStmtQueryInvoker> stmtStmtQueryInvoker;
  unique_ptr<PkbStmtEntQueryInvoker> stmtEntQueryInvoker;
  unique_ptr<PkbEntEntQueryInvoker> entEntQueryInvoker;

  unique_ptr<IDesignEntitiesQueryHandler> designEntityHandler;
  unique_ptr<IFollowsQueryHandler> followsHandler;
  unique_ptr<IFollowsQueryHandler> followsTHandler;
  unique_ptr<IParentQueryHandler> parentHandler;
  unique_ptr<IParentQueryHandler> parentTHandler;
  unique_ptr<IModifiesQueryHandler> modifiesHandler;
  unique_ptr<IUsesQueryHandler> usesHandler;
  unique_ptr<ICallsQueryHandler> callsHandler;
  unique_ptr<ICallsQueryHandler> callsTHandler;

  unique_ptr<IIfPatternQueryHandler> ifPatternHandler;
  unique_ptr<IWhilePatternQueryHandler> whilePatternHandler;
  unique_ptr<IAssignsQueryHandler> assignHandler;
  unique_ptr<ICFGsQueryHandler> cfgsHandler;
};
