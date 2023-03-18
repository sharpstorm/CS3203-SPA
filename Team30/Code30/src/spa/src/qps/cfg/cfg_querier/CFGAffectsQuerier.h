#pragma once

#include <unordered_set>
#include <unordered_map>

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/affects/CFGStatefulWalker.h"

using std::unordered_set, std::unordered_map;

typedef unordered_set<EntityValue> EntitySet;
typedef unordered_map<EntityValue, int> EntitySymbolMap;

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

  struct QueryToResultClosure {
    CFG* cfg;
    ClosureType* closure;
    StmtTransitiveResult* result;
    StmtValue endingStmt;
    EntitySymbolMap symbolMap;
  };

  CFG* cfg;
  CFGWalker walker;
  ClosureType* closure;

  bool validateArg(const StmtValue &arg);

  template <class T>
  static constexpr bool isContainer(T* closure, const int &stmtNumber) {
    return (typePredicate(closure, StmtType::While, stmtNumber)
        || typePredicate(closure, StmtType::If, stmtNumber));
  }

  static constexpr WalkerSingleCallback<QueryFromResultClosure>
      forwardWalkerCallback =
      [](QueryFromResultClosure *state, CFGNode nextNode) -> bool {
        int stmtNumber = 0;
        stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (typePredicate(state->closure, StmtType::Assign, stmtNumber)) {
          unordered_set<EntityValue> usedVars = usesGetter(state->closure,
                                                           stmtNumber);
          if (usedVars.find(state->target) != usedVars.end()) {
            state->result->add(state->startingStmt, stmtNumber);
          }
        }

        if (isContainer(state->closure, stmtNumber)) {
          return true;
        }
        return modifiesGetter(state->closure, stmtNumber) != state->target;
      };

  static constexpr StatefulWalkerStateTransformer<QueryToResultClosure>
      backwardWalkerTransformer =
      [](QueryToResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (isContainer(state->closure, stmtNumber)) {
          return curState;
        }

        EntityValue modifiedVar = modifiesGetter(state->closure, stmtNumber);
        if (auto it = state->symbolMap.find(modifiedVar);
            it != state->symbolMap.end()) {
          curState.unset(it->second);
        }

        return curState;
      };

  static constexpr StatefulWalkerSingleCallback<QueryToResultClosure>
      backwardWalkerCallback =
      [](QueryToResultClosure *state, CFGNode nextNode) {
        int stmtNumber = 0;
        stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (!typePredicate(state->closure, StmtType::Assign, stmtNumber)) {
          return;
        }
        state->result->add(stmtNumber, state->endingStmt);
      };
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

        if (isContainer(state->closure, stmtNumber)) {
          return true;
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
  walker.walkFrom<QueryFromResultClosure, forwardWalkerCallback>(nodeFrom,
                                                                 &state);
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
  if (!validateArg(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  EntitySet usedVars = usesGetter(closure, arg1);
  EntitySymbolMap symbolMap;
  if (usedVars.empty()) {
    return result;
  }

  int counter = 0;
  for (EntityValue val : usedVars) {
    symbolMap.emplace(val, counter);
    counter++;
  }

  BitField initialState(counter);
  for (int i = 0; i < counter; i++) {
    initialState.set(i);
  }

  QueryToResultClosure state { cfg, closure, &result, arg1, symbolMap };
  CFGStatefulWalker statefulWalker(cfg);

  statefulWalker.walkTo<QueryToResultClosure,
                        backwardWalkerCallback>(nodeTo,
                                                initialState,
                                                &state,
                                                backwardWalkerTransformer);
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
  for (int start = 0; start < cfg->getNodeCount(); start++) {
    int stmtNumber = cfg->fromCFGNode(start);
    if (!typePredicate(closure, StmtType::Assign, stmtNumber)) {
      continue;
    }

    EntityValue modifiedVar = modifiesGetter(closure, stmtNumber);
    QueryFromResultClosure state{ cfg, closure, resultOut,
                                  stmtNumber, modifiedVar };
    walker.walkFrom<QueryFromResultClosure, forwardWalkerCallback>(start,
                                                                   &state);
  }
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
