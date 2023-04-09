#pragma once

#include <vector>
#include <unordered_set>

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
class CFGAffectsTQuerier : public CFGQuerier<
    CFGAffectsTQuerier<ClosureType,
                       typePredicate,
                       modifiesGetter,
                       usesGetter>> {
  using ConcreteAffectsQuerier = CFGAffectsQuerier<ClosureType, typePredicate,
                                                   modifiesGetter, usesGetter>;
  template<class T>
  using DFSResultCallback = bool (*)(T *dfsClosure,
                                     const StmtValue &start,
                                     const StmtValue &stmt);
  template<class T, class U>
  using DFSStepGetter = U(*)(T *dfsClosure, const StmtValue &stmt);
  typedef unordered_set<StmtValue> AffectsResult;
  typedef vector<AffectsResult> AffectsResultList;
  typedef vector<StmtValue> StmtValueStack;
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

  struct BoolDFSClosure {
    StmtTransitiveResult *output;
    ConcreteAffectsQuerier affectsQuerier;
    StmtValue target;
  };

  struct SingleSynDFSClosure {
    StmtTransitiveResult *output;
    ConcreteAffectsQuerier affectsQuerier;
  };

  struct AllDFSClosure {
    StmtTransitiveResult *output;
    ConcreteAffectsQuerier affectsQuerier;
    const AffectsResultList *affectsResults;
    CFG *cfg;
  };

  bool validateArg(const StmtValue &arg);

  template<class DFSClosureType, class DFSStepType>
  void runAffectsTDFS(DFSResultCallback<DFSClosureType> callback,
                      DFSStepGetter<DFSClosureType, DFSStepType> stepGetter,
                      DFSClosureType *dfsClosure,
                      const StmtValue &start);

  template<class T>
  static constexpr DFSStepGetter<T, AffectsResult> affectsQueryFrom =
      [](T *dfsClosure, const StmtValue &stmt) {
        return dfsClosure->affectsQuerier.queryFrom(stmt, StmtType::Assign)
            .secondArgVals;
      };

  template<class T>
  static constexpr DFSStepGetter<T, AffectsResult> affectsQueryTo =
      [](T *dfsClosure, const StmtValue &stmt) {
        return dfsClosure->affectsQuerier.queryTo(StmtType::Assign, stmt)
            .firstArgVals;
      };
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

  BoolDFSClosure dfsClosure{&result, affectsQuerier, arg1};
  constexpr DFSResultCallback<BoolDFSClosure> resultHandler =
      [](BoolDFSClosure *dfsClosure,
         const StmtValue &start,
         const StmtValue &stmt) -> bool {
        if (stmt == dfsClosure->target) {
          dfsClosure->output->add(start, stmt);
          return false;
        }
        return true;
      };

  runAffectsTDFS(resultHandler, affectsQueryFrom<BoolDFSClosure>,
                 &dfsClosure, arg0);
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

  SingleSynDFSClosure dfsClosure{&result, affectsQuerier};
  constexpr DFSResultCallback<SingleSynDFSClosure> resultHandler =
      [](SingleSynDFSClosure *result,
         const StmtValue &start,
         const StmtValue &stmt) -> bool {
        result->output->add(start, stmt);
        return true;
      };

  runAffectsTDFS(resultHandler, affectsQueryFrom<SingleSynDFSClosure>,
                 &dfsClosure, arg0);
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

  SingleSynDFSClosure dfsClosure{&result, affectsQuerier};
  constexpr DFSResultCallback<SingleSynDFSClosure> resultHandler =
      [](SingleSynDFSClosure *result,
         const StmtValue &start,
         const StmtValue &stmt) -> bool {
        result->output->add(stmt, start);
        return true;
      };

  runAffectsTDFS(resultHandler, affectsQueryTo<SingleSynDFSClosure>,
                 &dfsClosure, arg1);
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
  AffectsResultList resultStmts;
  for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
    StmtTransitiveResult result = affectsQuerier.queryFrom(
        cfg->fromCFGNode(start), type1);
    resultStmts.push_back(result.secondArgVals);
  }

  AllDFSClosure dfsClosure{resultOut, affectsQuerier, &resultStmts, cfg};
  constexpr DFSResultCallback<AllDFSClosure> resultHandler =
      [](AllDFSClosure *closure,
         const StmtValue &start,
         const StmtValue &stmt) -> bool {
        closure->output->add(start, stmt);
        return true;
      };

  constexpr DFSStepGetter<AllDFSClosure, AffectsResult> stepGetter =
      [](AllDFSClosure *dfsClosure, const StmtValue &stmt) {
        CFGNode nodeId = dfsClosure->cfg->toCFGNode(stmt);
        return dfsClosure->affectsResults->at(nodeId);
      };

  for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
    StmtValue startStmt = cfg->fromCFGNode(start);
    runAffectsTDFS(resultHandler, stepGetter, &dfsClosure, startStmt);
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

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
template<class DFSClosureType, class DFSStepType>
void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter>::
runAffectsTDFS(DFSResultCallback<DFSClosureType> callback,
               DFSStepGetter<DFSClosureType, DFSStepType> stepGetter,
               DFSClosureType *dfsClosure,
               const StmtValue &start) {
  StmtValueStack dfsStack;
  BitField seen(cfg->getNodeCount());
  dfsStack.push_back(start);
  while (!dfsStack.empty()) {
    StmtValue curStmt = dfsStack.back();
    dfsStack.pop_back();

    DFSStepType affectsResult = stepGetter(dfsClosure, curStmt);
    for (const StmtValue &stmt : affectsResult) {
      CFGNode nextNode = cfg->toCFGNode(stmt);
      if (seen.isSet(nextNode)) {
        continue;
      }
      if (!callback(dfsClosure, start, stmt)) {
        return;
      }
      seen.set(nextNode);
      dfsStack.push_back(stmt);
    }
  }
}
