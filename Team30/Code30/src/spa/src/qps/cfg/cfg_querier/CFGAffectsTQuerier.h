#pragma once

#include <unordered_set>
#include <unordered_map>

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/walkers/CFGStatefulWalker.h"
#include "qps/cfg/cfg_querier/CFGQuerier.h"

using std::unordered_set, std::unordered_map;

typedef unordered_set<EntityValue> EntitySet;
typedef unordered_map<EntityValue, int> EntitySymbolMap;

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>
class CFGAffectsTQuerier: public ICFGClauseQuerier,
                          public CFGQuerier<
                              CFGAffectsTQuerier<ClosureType,
                                                 typePredicate,
                                                 modifiesGetter,
                                                 usesGetter,
                                                 countGetter,
                                                 symbolIdGetter>> {
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

  template <class T, StmtTypePredicate<T> typeChecker>
  static constexpr bool isContainer(const T &closure, const int &stmtNumber) {
    return (typeChecker(closure, StmtType::While, stmtNumber)
        || typeChecker(closure, StmtType::If, stmtNumber));
  }

 private:
  struct BoolResultClosure {
    CFG* cfg;
    const ClosureType &closure;
    EntityValue target;
    StmtValue targetStmt;
    bool isValidPathFound;
  };

  struct QueryFromResultClosure {
    CFG* cfg;
    const ClosureType &closure;
    StmtTransitiveResult* result;
    StmtValue startingStmt;
    EntityValue target;
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
};

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>
CFGAffectsTQuerier<ClosureType, typePredicate,
                   modifiesGetter, usesGetter,
                   countGetter, symbolIdGetter>::CFGAffectsTQuerier(
    CFG *cfg, const ClosureType &closure): cfg(cfg), walker(cfg),
                                           closure(closure) {}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter,
                                        countGetter, symbolIdGetter>::
queryBool(const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;
  CFGNode nodeFrom = cfg->toCFGNode(arg0);


  if (!validateArg(arg0) || !validateArg(arg1)) {
    return result;
  }

  EntityValue modifiedVar = modifiesGetter(closure, arg0);
  BoolResultClosure state{ cfg, closure, modifiedVar, arg1, false };

  int countSymbols = countGetter(closure);
  BitField initialState(countSymbols);
  initialState.set(symbolIdGetter(closure, modifiedVar));

  constexpr StatefulWalkerSingleCallback<BoolResultClosure>
      forwardWalkerCallback =
      [](BoolResultClosure *state, CFGNode nextNode) {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);
        if (stmtNumber == state->targetStmt) {
          state->isValidPathFound = true;
          throw CFGHaltWalkerException();
        }
      };

  constexpr StatefulWalkerStateTransformer<BoolResultClosure>
      forwardWalkerTransformer =
      [](BoolResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return curState;
        }

        EntitySet usedVar = usesGetter(state->closure, stmtNumber);
        for (EntityValue var : usedVar) {
          int id = symbolIdGetter(state->closure, var);
          if (curState.isSet(id)) {
            EntityValue modifiedVar = modifiesGetter(state->closure, stmtNumber);
            curState.set(symbolIdGetter(state->closure, modifiedVar));
            break;
          }
        }
        return curState;
      };

  CFGStatefulWalker statefulWalker(cfg);
  statefulWalker.walkFrom<BoolResultClosure,
                          forwardWalkerCallback>(nodeFrom,
                                                 initialState,
                                                 &state,
                                                 forwardWalkerTransformer);
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
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter
>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter,
                                        countGetter, symbolIdGetter>::
queryFrom(const StmtValue &arg0, const StmtType &type1) {
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
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>
StmtTransitiveResult CFGAffectsTQuerier<ClosureType, typePredicate,
                                        modifiesGetter, usesGetter,
                                        countGetter, symbolIdGetter>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;
  if (!validateArg(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  EntitySet usedVars = usesGetter(closure, arg1);

  int countSymbols = countGetter(closure);
  BitField initialState(countSymbols);
  for (EntityValue x : usedVars) {
    initialState.set(symbolIdGetter(closure, x));
  }

  constexpr StatefulWalkerSingleCallback<QueryToResultClosure>
      backwardWalkerCallback =
      [](QueryToResultClosure *state, CFGNode nextNode) {
        int stmtNumber = 0;
        stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (!typePredicate(state->closure, StmtType::Assign, stmtNumber)) {
          return;
        }


        state->result->add(stmtNumber, state->endingStmt);
      };

  constexpr StatefulWalkerStateTransformer<QueryToResultClosure>
      backwardWalkerTransformer =
      [](QueryToResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return curState;
        }

        EntityValue modifiedVar = modifiesGetter(state->closure, stmtNumber);
        int symId = symbolIdGetter(state->closure, modifiedVar);
        if (curState.isSet(symId)) {
          curState.unset(symId);
          EntityValueSet usedVars = usesGetter(state->closure, stmtNumber);
          for (EntityValue var : usedVars) {
            curState.set(symbolIdGetter(state->closure, var));
          }
        }

        return curState;
      };

  QueryToResultClosure state { cfg, closure, &result, arg1 };
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
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>
void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter,
                        countGetter, symbolIdGetter>::
queryAll(StmtTransitiveResult* resultOut,
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
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>

void CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter,
                        countGetter, symbolIdGetter>::
queryForward(StmtTransitiveResult* resultOut,
             const StmtValue &start) {

  int stmtNumber = cfg->fromCFGNode(start);

  if (!validateArg(stmtNumber)) {
    return;
  }

  EntityValue modifiedVar = modifiesGetter(closure, stmtNumber);
  QueryFromResultClosure state{ cfg, closure, resultOut,
                                stmtNumber, modifiedVar };

  int countSymbols = countGetter(closure);
  BitField initialState(countSymbols);
  initialState.set(symbolIdGetter(closure, modifiedVar));

  constexpr StatefulWalkerSingleCallback<QueryFromResultClosure>
      forwardWalkerCallback =
      [](QueryFromResultClosure *state, CFGNode nextNode)  {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);

//        if (typePredicate(state->closure, StmtType::Assign, stmtNumber)) {
//          unordered_set<EntityValue> usedVars = usesGetter(state->closure,
//                                                           stmtNumber);
//          if (usedVars.find(state->target) != usedVars.end()) {
          state->result->add(state->startingStmt, stmtNumber);
//          }
//        }
      };


  constexpr StatefulWalkerStateTransformer<QueryFromResultClosure>
      forwardWalkerTransformer =
      [](QueryFromResultClosure *state, CFGNode nextNode, BitField curState)
          -> BitField {
        int stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return curState;
        }

        EntitySet usedVar = usesGetter(state->closure, stmtNumber);
        for (EntityValue var : usedVar) {
          int id = symbolIdGetter(state->closure, var);
          if (curState.isSet(id)) {
            EntityValue modifiedVar = modifiesGetter(state->closure, stmtNumber);
            curState.set(symbolIdGetter(state->closure, modifiedVar));
            break;
          }
        }

        return curState;
      };

  CFGStatefulWalker statefulWalker(cfg);
  statefulWalker.walkFrom<QueryFromResultClosure,
                          forwardWalkerCallback>(start,
                                                 initialState,
                                                 &state,
                                                 forwardWalkerTransformer);
}

template <
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter,
    CountGetter<ClosureType> countGetter,
    SymbolIdGetter<ClosureType> symbolIdGetter>
bool CFGAffectsTQuerier<ClosureType, typePredicate,
                        modifiesGetter, usesGetter,
                        countGetter, symbolIdGetter>::
validateArg(const StmtValue &arg) {
  return cfg->containsStatement(arg)
      && typePredicate(closure, StmtType::Assign, arg);
}
