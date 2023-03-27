#pragma once

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/walkers/CFGStatefulWalker.h"
#include "qps/cfg/cfg_querier/CFGQuerier.h"

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
class CFGAffectsTQuerier: public ICFGClauseQuerier,
                          public CFGQuerier<
                              CFGAffectsTQuerier<ClosureType,
                                                 typePredicate,
                                                 modifiesGetter,
                                                 usesGetter,
                                                 countGetter>> {
 public:
  explicit CFGAffectsTQuerier(CFG* cfg, const ClosureType &closure);

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
    const ClosureType &closure;
    EntityIdx target;
    StmtValue targetStmt;
    bool isValidPathFound;
  };

  struct QueryFromResultClosure {
    CFG* cfg;
    const ClosureType &closure;
    StmtTransitiveResult* result;
    StmtValue startingStmt;
    EntityIdx target;
  };

  struct QueryToResultClosure {
    CFG* cfg;
    const ClosureType &closure;
    StmtTransitiveResult* result;
    StmtValue endingStmt;
  };

  CFG* cfg;
  CFGWalker walker;
  const ClosureType &closure;

  bool validateArg(const StmtValue &arg);

  void queryForward(StmtTransitiveResult* resultOut,
                    const StmtValue &start);

  template <class T, StmtTypePredicate<T> typeChecker>
  static constexpr bool isContainer(const T &closure, const int &stmtNumber) {
    return (typeChecker(closure, StmtType::While, stmtNumber)
        || typeChecker(closure, StmtType::If, stmtNumber));
  }

  template <class T, StmtTypePredicate<T> typeChecker>
  static constexpr bool isAssign(const T &closure, const int &stmtNumber) {
    return typeChecker(closure, StmtType::Assign, stmtNumber);
  }
};

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
CFGAffectsTQuerier<ClosureType, typePredicate,
                   modifiesGetter, usesGetter,
                   countGetter>::CFGAffectsTQuerier(
    CFG *cfg, const ClosureType &closure): cfg(cfg), walker(cfg),
                                           closure(closure) {}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter,
                                        countGetter>::queryBool(
    const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;
  CFGNode nodeFrom = cfg->toCFGNode(arg0);


  if (!validateArg(arg0) || !validateArg(arg1)) {
    return result;
  }

  EntityIdx modifiedVar = modifiesGetter(closure, arg0);
  BoolResultClosure state{ cfg, closure, modifiedVar, arg1, false };

  int countSymbols = countGetter(closure);
  BitField initialState(countSymbols);
  initialState.set(modifiedVar);

  constexpr StatefulWalkerSingleCallback<BoolResultClosure>
      forwardWalkerCallback =
      [](BoolResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);
        if (isContainer<ClosureType, typePredicate>(state->closure,
                                                    stmtNumber)) {
          return curState;
        }

        EntityIdx modifiedVar = modifiesGetter(state->closure, stmtNumber);
        if (modifiedVar == NO_ENT_INDEX) {
          // Only case with no modifications are print & containers
          return curState;
        }

        bool isUsed = false;
        EntityIdxSet usedVars = usesGetter(state->closure, stmtNumber);
        for (EntityIdx varId : usedVars) {
          if (curState.isSet(varId)) {
            isUsed = true;
            break;
          }
        }

        curState.unset(modifiedVar);
        if (!isUsed) {
          return curState;
        }
        curState.set(modifiedVar);

        if (stmtNumber == state->targetStmt) {
          state->isValidPathFound = true;
          throw CFGHaltWalkerException();
        }

        return curState;
      };

  CFGStatefulWalker statefulWalker(cfg);
  statefulWalker.walkFrom<BoolResultClosure,
                          forwardWalkerCallback>(nodeFrom,
                                                 initialState,
                                                 &state);
  if (state.isValidPathFound) {
    result.add(arg0, arg1);
  }
  return result;
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter,
                                        countGetter>::queryFrom(
    const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  queryForward(&result, nodeFrom);
  return result;
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter,
                                        countGetter>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;
  if (!validateArg(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  EntityIdxSet usedVars = usesGetter(closure, arg1);

  int countSymbols = countGetter(closure);
  BitField initialState(countSymbols);
  for (EntityIdx x : usedVars) {
    initialState.set(x);
  }

  constexpr StatefulWalkerSingleCallback<QueryToResultClosure>
      backwardWalkerCallback =
      [](QueryToResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);
        bool isAffected = false;

        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return curState;
        }

        EntityIdx modifiedEntId = modifiesGetter(state->closure, stmtNumber);
        if (curState.isSet(modifiedEntId)) {
          curState.unset(modifiedEntId);
          EntityIdxSet usedVars = usesGetter(state->closure, stmtNumber);
          for (EntityIdx var : usedVars) {
            curState.set(var);
          }
          isAffected = isAssign<ClosureType, typePredicate>(state->closure,
                                                            stmtNumber);
        }

        if (isAffected) {
          state->result->add(stmtNumber, state->endingStmt);
        }

        return curState;
      };

  QueryToResultClosure state { cfg, closure, &result, arg1 };
  CFGStatefulWalker statefulWalker(cfg);

  statefulWalker.walkTo<QueryToResultClosure,
                        backwardWalkerCallback>(nodeTo,
                                                initialState,
                                                &state);
  return result;
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter,
                        countGetter>::queryAll(StmtTransitiveResult* resultOut,
                                               const StmtType &type0,
                                               const StmtType &type1) {
  for (int start = 0; start < cfg->getNodeCount(); start++) {
    queryForward(resultOut, start);
  }
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter,
                        countGetter>::queryForward(
    StmtTransitiveResult* resultOut, const StmtValue &start) {
  int stmtNumber = cfg->fromCFGNode(start);

  if (!validateArg(stmtNumber)) {
    return;
  }

  EntityIdx modifiedVar = modifiesGetter(closure, stmtNumber);
  QueryFromResultClosure state{ cfg, closure, resultOut,
                                stmtNumber, modifiedVar };

  int countSymbols = countGetter(closure);
  BitField initialState(countSymbols);
  initialState.set(modifiedVar);

  constexpr StatefulWalkerSingleCallback<QueryFromResultClosure>
      forwardWalkerCallback =
      [](QueryFromResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);
        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return curState;
        }

        EntityIdx modifiedVar = modifiesGetter(state->closure, stmtNumber);
        if (modifiedVar == NO_ENT_INDEX) {
          // Only case with no modifications are print & containers
          return curState;
        }

        bool isUsed = false;
        EntityIdxSet usedVars = usesGetter(state->closure, stmtNumber);
        for (EntityIdx varId : usedVars) {
          if (curState.isSet(varId)) {
            isUsed = true;
            break;
          }
        }

        curState.unset(modifiedVar);
        if (!isUsed) {
          return curState;
        }
        curState.set(modifiedVar);
        state->result->add(state->startingStmt, stmtNumber);
        return curState;
      };

  CFGStatefulWalker statefulWalker(cfg);
  statefulWalker.walkFrom<QueryFromResultClosure,
                          forwardWalkerCallback>(start,
                                                 initialState,
                                                 &state);
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter>
bool CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter,
                        countGetter>::validateArg(const StmtValue &arg) {
  return cfg->containsStatement(arg)
      && typePredicate(closure, StmtType::Assign, arg);
}
