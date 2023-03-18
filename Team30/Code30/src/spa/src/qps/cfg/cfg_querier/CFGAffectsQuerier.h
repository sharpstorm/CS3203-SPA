#pragma once

#include <unordered_set>

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
class CFGAffectsQuerier: public ICFGClauseQuerier,
                         public CFGQuerier<
                             CFGAffectsQuerier<ClosureType,
                                               typePredicate,
                                               modifiesGetter,
                                               usesGetter>> {
 public:
  explicit CFGAffectsQuerier(CFG* cfg, ClosureType* closure);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) final;
  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1) final;
  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1) final;
  void queryAll(StmtTransitiveResult* resultOut,
                const StmtType &type0,
                const StmtType &type1) final;

 private:
  struct BoolResultClosure {
    CFG* cfg;
    ClosureType* closure;
    EntityValue target;
    StmtValue targetStmt;
    bool isValidPathFound;
  };

  struct QueryFromResultClosure {
    CFG* cfg;
    ClosureType* closure;
    StmtTransitiveResult* result;
    StmtValue startingStmt;
    EntityValue target;
  };


  CFG* cfg;
  CFGWalker walker;
  ClosureType* closure;

  bool validateArg(const StmtValue &arg);
};

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
CFGAffectsQuerier<ClosureType, typePredicate,
                  modifiesGetter, usesGetter>::CFGAffectsQuerier(
    CFG *cfg, ClosureType* closure): cfg(cfg), walker(cfg), closure(closure) {}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsQuerier<ClosureType, typePredicate,
                                       modifiesGetter, usesGetter>::
queryBool(const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!validateArg(arg0) || !validateArg(arg1)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);

  EntityValue modifiedVar = modifiesGetter(closure, arg0);
  unordered_set<EntityValue> targetVars = usesGetter(closure, arg1);
  if (targetVars.find(modifiedVar) == targetVars.end()) {
    return result;
  }

  BoolResultClosure state{ cfg, closure, modifiedVar, arg1, false };
  constexpr WalkerSingleCallback<BoolResultClosure> callback =
      [](BoolResultClosure *state, CFGNode nextNode) -> bool {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);
        if (stmtNumber == state->targetStmt) {
          state->isValidPathFound = true;
          throw CFGHaltWalkerException();
        }

        return modifiesGetter(state->closure, stmtNumber) != state->target;
      };

  walker.walkFrom<BoolResultClosure, callback>(nodeFrom, &state);
  if (state.isValidPathFound) {
    result.add(arg0, arg1);
  }
  return result;
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsQuerier<ClosureType, typePredicate,
                                       modifiesGetter, usesGetter>::
queryFrom(const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;

  if (!validateArg(arg0)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  EntityValue modifiedVar = modifiesGetter(closure, arg0);


  QueryFromResultClosure state{ cfg, closure, &result, arg0, modifiedVar };
  constexpr WalkerSingleCallback<QueryFromResultClosure> callback =
      [](QueryFromResultClosure *state, CFGNode nextNode) -> bool {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (typePredicate(state->closure, StmtType::Assign, stmtNumber)) {
          unordered_set<EntityValue> usedVars = usesGetter(state->closure,
                                                           stmtNumber);
          if (usedVars.find(state->target) != usedVars.end()) {
            state->result->add(state->startingStmt, stmtNumber);
          }
        }

        return modifiesGetter(state->closure, stmtNumber) != state->target;
      };

  walker.walkFrom<QueryFromResultClosure, callback>(nodeFrom, &state);
  return result;
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsQuerier<ClosureType, typePredicate,
                                       modifiesGetter, usesGetter>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  return result;
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
void CFGAffectsQuerier<ClosureType, typePredicate,
                       modifiesGetter, usesGetter>::
queryAll(StmtTransitiveResult* resultOut,
         const StmtType &type0,
         const StmtType &type1) {
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
bool CFGAffectsQuerier<ClosureType, typePredicate,
                       modifiesGetter, usesGetter>::
validateArg(const StmtValue &arg) {
  return cfg->containsStatement(arg)
      && typePredicate(closure, StmtType::Assign, arg);
}
