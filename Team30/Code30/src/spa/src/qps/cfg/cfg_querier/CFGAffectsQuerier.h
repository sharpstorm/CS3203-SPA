#pragma once

#include <map>

#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/walkers/CFGStatefulWalker.h"
#include "common/SetUtils.h"
#include "qps/common/CacheTable.h"

using std::map;

typedef map<EntityIdx, int> EntitySymbolMap;

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
class CFGAffectsQuerier : public CFGQuerier<
    CFGAffectsQuerier<ClosureType,
                      typePredicate,
                      modifiesGetter,
                      usesGetter>> {
 public:
  explicit CFGAffectsQuerier(CFG *cfg, const ClosureType &closure);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1);
  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1);
  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1);
  void queryAll(StmtTransitiveResult *resultOut,
                const StmtType &type0,
                const StmtType &type1);

  template<class T, StmtTypePredicate<T> typeChecker>
  static constexpr bool isContainer(const T &closure,
                                    const StmtValue &stmtNumber) {
    return (typeChecker(closure, StmtType::While, stmtNumber)
        || typeChecker(closure, StmtType::If, stmtNumber));
  }

 private:
  struct BoolResultClosure {
    CFG *cfg;
    const ClosureType &closure;
    EntityIdx target;
    StmtValue targetStmt;
    bool isValidPathFound;
  };

  struct QueryFromResultClosure {
    CFG *cfg;
    const ClosureType &closure;
    StmtTransitiveResult *result;
    StmtValue startingStmt;
    EntityIdx target;
  };

  struct QueryToResultClosure {
    CFG *cfg;
    const ClosureType &closure;
    StmtTransitiveResult *result;
    StmtValue endingStmt;
    EntitySymbolMap symbolMap;
  };

  CFG *cfg;
  CFGWalker walker;
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
CFGAffectsQuerier<ClosureType, typePredicate,
                  modifiesGetter, usesGetter>::CFGAffectsQuerier(
    CFG *cfg, const ClosureType &closure): cfg(cfg), walker(cfg),
                                           closure(closure) {}

template<
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

  CacheTable *cacheTable = closure.getAffectsCache();
  if (cacheTable->queryPartial(arg0, arg1) != nullptr) {
    result.add(arg0, arg1);
    return result;
  }

  auto row = cacheTable->queryFull(arg0, 0);
  if (row != nullptr) {
    if (std::find(row->begin(), row->end(), arg1) != row->end()) {
      result.add(arg0, arg1);
      return result;
    }
  }

  row = cacheTable->queryFull(0, arg1);
  if (row != nullptr) {
    if (std::find(row->begin(), row->end(), arg0) != row->end()) {
      result.add(arg0, arg1);
      return result;
    }
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);

  EntityIdxSet modifiedVars = modifiesGetter(closure, arg0);
  EntityIdx modifiedVar = SetUtils::firstItemOfSet(modifiedVars, NO_ENT_INDEX);
  EntityIdxSet targetVars = usesGetter(closure, arg1);
  if (targetVars.find(modifiedVar) == targetVars.end()) {
    return result;
  }

  BoolResultClosure state{cfg, closure, modifiedVar, arg1, false};
  constexpr WalkerSingleCallback<BoolResultClosure> callback =
      [](BoolResultClosure *state, CFGNode nextNode) -> bool {
        StmtValue stmtNumber = state->cfg->fromCFGNode(nextNode);
        if (stmtNumber == state->targetStmt) {
          state->isValidPathFound = true;
          throw CFGHaltWalkerException();
        }

        if (CFGAffectsQuerier::isContainer<ClosureType, typePredicate>(
            state->closure, stmtNumber)) {
          return true;
        }
        return !SetUtils::setContains(
            modifiesGetter(state->closure, stmtNumber),
            state->target);
      };

  walker.walkFrom<BoolResultClosure, callback>(nodeFrom, &state);
  if (state.isValidPathFound) {
    cacheTable->addEntry(arg0, arg1);
    result.add(arg0, arg1);
  }
  return result;
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
StmtTransitiveResult CFGAffectsQuerier<ClosureType, typePredicate,
                                       modifiesGetter, usesGetter>::
queryFrom(const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;
  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CacheTable *cacheTable = closure.getAffectsCache();
  auto row = cacheTable->queryFull(arg0, 0);
  if (row != nullptr) {
    for (const StmtValue &i : *row) {
      result.add(arg0, i);
    }
    return result;
  }
  queryForward(&result, nodeFrom);
  return result;
}

template<
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

  CacheTable *cacheTable = closure.getAffectsCache();
  auto row = cacheTable->queryFull(0, arg1);
  if (row != nullptr) {
    for (const StmtValue &i : *row) {
      result.add(i, arg1);
    }
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  EntityIdxSet usedVars = usesGetter(closure, arg1);
  EntitySymbolMap symbolMap;
  if (usedVars.empty()) {
    return result;
  }

  int counter = 0;
  for (const EntityIdx &idx : usedVars) {
    symbolMap.emplace(idx, counter);
    counter++;
  }

  BitField initialState(counter);
  for (int i = 0; i < counter; i++) {
    initialState.set(i);
  }

  constexpr StatefulWalkerSingleCallback<QueryToResultClosure>
      backwardWalkerCallback =
      [](QueryToResultClosure *state, CFGNode nextNode, BitField *curState)
          -> void {
        bool isAffected = false;
        StmtValue stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return;
        }

        EntityIdxSet modifiedVars = modifiesGetter(state->closure, stmtNumber);
        for (const EntityIdx &item : modifiedVars) {
          auto it = state->symbolMap.find(item);
          if (it == state->symbolMap.end()) {
            continue;
          }

          if (!curState->isSet(it->second)) {
            continue;
          }
          curState->unset(it->second);
          isAffected = isAffected || typePredicate(state->closure,
                                                   StmtType::Assign,
                                                   stmtNumber);
        }

        if (isAffected) {
          state->result->add(stmtNumber, state->endingStmt);
          state->closure.getAffectsCache()->
              addEntry(stmtNumber, state->endingStmt);
        }

        return;
      };

  QueryToResultClosure state{cfg, closure, &result, arg1, symbolMap};
  CFGStatefulWalker<QueryToResultClosure,
                    backwardWalkerCallback> statefulWalker(cfg, &state);
  statefulWalker.walkTo(nodeTo, initialState);

  closure.getAffectsCache()->promoteTo(arg1);
  return result;
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
void CFGAffectsQuerier<ClosureType, typePredicate,
                       modifiesGetter, usesGetter>::
queryAll(StmtTransitiveResult *resultOut,
         const StmtType &type0,
         const StmtType &type1) {
  CacheTable *cacheTable = closure.getAffectsCache();
  for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
    StmtValue stmtNumber = cfg->fromCFGNode(start);
    auto row = cacheTable->queryFull(stmtNumber, 0);
    if (row != nullptr) {
      for (const StmtValue &i : *row) {
        resultOut->add(stmtNumber, i);
      }
      continue;
    }
    queryForward(resultOut, start);
  }
}

template<
    class ClosureType,
    StmtTypePredicate<ClosureType> typePredicate,
    ModifiesGetter<ClosureType> modifiesGetter,
    UsesGetter<ClosureType> usesGetter>
void CFGAffectsQuerier<ClosureType, typePredicate,
                       modifiesGetter, usesGetter>::
queryForward(StmtTransitiveResult *resultOut,
             const StmtValue &start) {
  StmtValue stmtNumber = cfg->fromCFGNode(start);
  if (!validateArg(stmtNumber)) {
    return;
  }

  constexpr WalkerSingleCallback<QueryFromResultClosure> forwardWalkerCallback
      = [](QueryFromResultClosure *state, CFGNode nextNode) -> bool {
        StmtValue stmtNumber = 0;
        stmtNumber = state->cfg->fromCFGNode(nextNode);

        if (typePredicate(state->closure, StmtType::Assign, stmtNumber)) {
          EntityIdxSet usedVars = usesGetter(state->closure,
                                             stmtNumber);
          if (usedVars.find(state->target) != usedVars.end()) {
            state->closure.getAffectsCache()->
                addEntry(state->startingStmt, stmtNumber);
            state->result->add(state->startingStmt, stmtNumber);
          }
        }

        if (isContainer<ClosureType,
                        typePredicate>(state->closure, stmtNumber)) {
          return true;
        }

        return !SetUtils::setContains(
            modifiesGetter(state->closure, stmtNumber),
            state->target);
      };

  EntityIdxSet modifiedVars = modifiesGetter(closure, stmtNumber);
  EntityIdx modifiedVar = SetUtils::firstItemOfSet(modifiedVars, NO_ENT_INDEX);
  QueryFromResultClosure state{cfg, closure, resultOut,
                               stmtNumber, modifiedVar};
  walker.walkFrom<QueryFromResultClosure, forwardWalkerCallback>(start,
                                                                 &state);
  closure.getAffectsCache()->promoteFrom(stmtNumber);
}

template<
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
