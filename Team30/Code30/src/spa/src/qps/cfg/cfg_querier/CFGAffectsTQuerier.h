#pragma once

#include <vector>
#include <unordered_set>

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/walkers/CFGStatefulWalker.h"
#include "qps/cfg/cfg_querier/CFGQuerier.h"
#include "common/SetUtils.h"
#include "CFGAffectsQuerier.h"

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
class CFGAffectsTQuerier : public ICFGClauseQuerier,
                           public CFGQuerier<
                               CFGAffectsTQuerier<ClosureType,
                                                  typePredicate,
                                                  modifiesGetter,
                                                  usesGetter>> {
  using ConcreteAffectsQuerier = CFGAffectsQuerier<ClosureType, typePredicate,
                                                   modifiesGetter, usesGetter>;
 public:
  explicit CFGAffectsTQuerier(CFG *cfg, const ClosureType &closure);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) final;
  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1) final;
  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1) final;
  void queryAll(StmtTransitiveResult *resultOut,
                const StmtType &type0,
                const StmtType &type1) final;

 private:
  CFG *cfg;
  ConcreteAffectsQuerier affectsQuerier;
  const ClosureType &closure;

  bool validateArg(const StmtValue &arg);
  void queryForward(StmtTransitiveResult *resultOut,
                    const StmtValue &start);
};

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
CFGAffectsTQuerier<ClosureType, typePredicate,
                   modifiesGetter, usesGetter>::CFGAffectsTQuerier(
    CFG *cfg, const ClosureType &closure): cfg(cfg), closure(closure),
                                           affectsQuerier(cfg, closure) {}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter>::
queryBool(const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!validateArg(arg0) || !validateArg(arg1)) {
    return result;
  }

  vector<StmtValue> dfsStack;
  BitField seen(cfg->getNodeCount());
  dfsStack.push_back(arg0);
  while (!dfsStack.empty()) {
    StmtValue curStmt = dfsStack.back();
    dfsStack.pop_back();

    StmtTransitiveResult affectsResult = affectsQuerier
        .queryFrom(curStmt, StmtType::Assign);
    for (const StmtValue &stmt : affectsResult.secondArgVals) {
      CFGNode nextNode = cfg->toCFGNode(stmt);
      if (seen.isSet(nextNode)) {
        continue;
      }
      if (stmt == arg1) {
        result.add(arg0, arg1);
        return result;
      }

      seen.set(nextNode);
      dfsStack.push_back(stmt);
    }
  }
  return result;
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter>::queryFrom(
    const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;

  queryForward(&result, arg0);
  return result;
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;
  if (!validateArg(arg1)) {
    return result;
  }

  vector<StmtValue> dfsStack;
  BitField seen(cfg->getNodeCount());
  dfsStack.push_back(arg1);
  while (!dfsStack.empty()) {
    StmtValue curStmt = dfsStack.back();
    dfsStack.pop_back();

    StmtTransitiveResult affectsResult = affectsQuerier.queryTo(type0, curStmt);
    for (const StmtValue &stmt : affectsResult.firstArgVals) {
      CFGNode nextNode = cfg->toCFGNode(stmt);
      if (seen.isSet(nextNode)) {
        continue;
      }
      result.add(stmt, arg1);
      seen.set(nextNode);
      dfsStack.push_back(stmt);
    }
  }

  return result;
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter>::
queryAll(StmtTransitiveResult *resultOut,
         const StmtType &type0,
         const StmtType &type1) {
  vector<unordered_set<StmtValue>> resultStmts;
  for (int start = 0; start < cfg->getNodeCount(); start++) {
    auto result = affectsQuerier.queryFrom(cfg->fromCFGNode(start), type1);
    resultStmts.push_back(result.secondArgVals);
  }

  for (int start = 0; start < cfg->getNodeCount(); start++) {
    StmtValue startStmt = cfg->fromCFGNode(start);
    vector<CFGNode> dfsStack;
    BitField seen(cfg->getNodeCount());
    dfsStack.push_back(start);
    while (!dfsStack.empty()) {
      CFGNode curNode = dfsStack.back();
      dfsStack.pop_back();

      for (const StmtValue &stmt : resultStmts.at(curNode)) {
        CFGNode nextNode = cfg->toCFGNode(stmt);
        if (seen.isSet(nextNode)) {
          continue;
        }
        seen.set(nextNode);
        resultOut->add(startStmt, stmt);
        dfsStack.push_back(nextNode);
      }
    }
  }
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter>::queryForward(
    StmtTransitiveResult *resultOut, const StmtValue &start) {
  if (!validateArg(start)) {
    return;
  }

  vector<StmtValue> dfsStack;
  BitField seen(cfg->getNodeCount());
  dfsStack.push_back(start);
  while (!dfsStack.empty()) {
    StmtValue curStmt = dfsStack.back();
    dfsStack.pop_back();

    StmtTransitiveResult affectsResult = affectsQuerier
        .queryFrom(curStmt,
                   StmtType::Assign);
    for (const StmtValue &stmt : affectsResult.secondArgVals) {
      CFGNode nextNode = cfg->toCFGNode(stmt);
      if (seen.isSet(nextNode)) {
        continue;
      }
      resultOut->add(start, stmt);
      seen.set(nextNode);
      dfsStack.push_back(stmt);
    }
  }
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
bool CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter>::
validateArg(const StmtValue &arg) {
  return cfg->containsStatement(arg)
      && typePredicate(closure, StmtType::Assign, arg);
}
