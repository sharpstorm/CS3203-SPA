#pragma once

#include <map>

#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/walkers/CFGStatefulWalker.h"
#include "common/SetUtils.h"
#include "qps/common/CacheTable.h"
#include "qps/cfg/cfg_querier/writers/ICFGWriter.h"
#include "qps/cfg/cfg_querier/writers/CFGBaseResultWriter.h"
#include "qps/cfg/cfg_querier/writers/CFGResultWriterFactory.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

using std::map;

typedef int BitPosition;
typedef map<EntityIdx, BitPosition> EntitySymbolMap;

template<class QuerierType>
class CFGAffectsQuerier : public CFGQuerier<CFGAffectsQuerier<QuerierType>> {
 private:
  QuerierType querier;
  CFG *cfg;
  CFGWalker walker;

 public:
  explicit CFGAffectsQuerier(CFG *cfg, QuerierType querier);

  void queryBool(StmtTransitiveResult *result, const StmtValue &arg0,
                 const StmtValue &arg1);
  void queryFrom(StmtTransitiveResult *result, const StmtValue &arg0,
                 const StmtType &type1);
  void queryTo(StmtTransitiveResult *result, const StmtType &type0,
               const StmtValue &arg1);
  void queryAll(StmtTransitiveResult *resultOut,
                const StmtType &type0,
                const StmtType &type1);
  static constexpr bool isContainer(QuerierType *querier,
                                    const StmtValue &stmtNumber) {
    return (querier->isStmtType(StmtType::While, stmtNumber)
        || querier->isStmtType(StmtType::If, stmtNumber));
  }

  bool validateArg(const StmtValue &arg) const;

 private:
  void queryForward(ICFGWriter *writer, const CFGNode &start);
  struct QueryState {
    ICFGWriter *writer;
    QuerierType *querier;
    EntityIdx target;
  };

  struct QueryToQueryState : public QueryState {
    EntitySymbolMap *symbolMap;
  };

  static constexpr TypePredicate<QuerierType> dummyTypePredicate =
      [](const QuerierType &state, StmtType type,
         StmtValue value) -> bool {
        return true;
      };
};

template<class T>
CFGAffectsQuerier<T>::CFGAffectsQuerier(CFG *cfg, T querier):
    cfg(cfg), walker(cfg), querier(querier) {}

template<class T>
void CFGAffectsQuerier<T>::queryBool(StmtTransitiveResult *result,
                                     const StmtValue &arg0,
                                     const StmtValue &arg1) {
  if (!validateArg(arg0) || !validateArg(arg1)) {
    return;
  }

  if (querier.queryAffectsPartial(arg0, arg1)) {
    result->setNotEmpty();
    return;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  EntityIdxSet modifiedVars = querier.getModifies(arg0);
  EntityIdx modifiedVar = SetUtils::firstItemOfSet(modifiedVars, NO_ENT_INDEX);
  EntityIdxSet targetVars = querier.getUses(arg1);
  if (targetVars.find(modifiedVar) == targetVars.end()) {
    return;
  }

  ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &querier, result)
      .template makeBoolWriter<CFGAffectsQuerier::dummyTypePredicate>(arg0,
                                                                      arg1);
  QueryState queryState{writer.get(), &querier, modifiedVar};

  constexpr WalkerSingleCallback<QueryState> callback =
      [](QueryState *queryState, CFGNode nextNode) -> bool {
        StmtValue stmtNumber = queryState->writer->toStmtNumber(nextNode);
        if (!queryState->writer->writeBool(stmtNumber)) {
          throw CFGHaltWalkerException();
        }

        if (CFGAffectsQuerier::isContainer(queryState->querier, stmtNumber)) {
          return true;
        }
        return !SetUtils::setContains(
            queryState->querier->getModifies(stmtNumber),
            queryState->target);
      };

  walker.walkFrom<QueryState, callback>(nodeFrom, &queryState);
}

template<class T>
void CFGAffectsQuerier<T>::queryFrom(StmtTransitiveResult *result,
                                     const StmtValue &arg0,
                                     const StmtType &type1) {
  if (!validateArg(arg0)) {
    return;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);

  ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &querier, result)
      .template makeRightWriter<CFGAffectsQuerier::dummyTypePredicate>(arg0,
                                                                       type1);
  auto row = querier.queryAffectsFull(arg0, 0);
  if (row != nullptr) {
    for (const StmtValue &i : *row) {
      writer->writeRight(i);
    }
    return;
  }

  queryForward(writer.get(), nodeFrom);
  if (type1 != StmtType::Wildcard) {
    querier.promoteAffectsFrom(arg0);
  }
}

template<class T>
void CFGAffectsQuerier<T>::queryTo(StmtTransitiveResult *result,
                                   const StmtType &type0,
                                   const StmtValue &arg1) {
  if (!validateArg(arg1)) {
    return;
  }

  ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &querier, result)
      .template makeLeftWriter<CFGAffectsQuerier::dummyTypePredicate>(type0,
                                                                      arg1);
  auto row = querier.queryAffectsFull(0, arg1);
  if (row != nullptr) {
    for (const StmtValue &i : *row) {
      writer->writeLeft(i);
    }
    return;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  EntityIdxSet usedVars = querier.getUses(arg1);
  EntitySymbolMap symbolMap;
  if (usedVars.empty()) {
    return;
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

  QueryToQueryState queryState{writer.get(), &querier,
                               NO_ENT_INDEX, &symbolMap};
  constexpr StatefulWalkerSingleCallback<QueryToQueryState>
      backwardWalkerCallback = [](QueryToQueryState *state, CFGNode nextNode,
                                  BitField *curState) -> void {
    bool isAffected = false;
    StmtValue stmtNumber = state->writer->toStmtNumber(nextNode);

    if (CFGAffectsQuerier::isContainer(state->querier, stmtNumber)) {
      return;
    }

    EntityIdxSet modifiedVars = state->querier->getModifies(stmtNumber);
    for (const EntityIdx &item : modifiedVars) {
      auto it = state->symbolMap->find(item);
      if (it == state->symbolMap->end()) {
        continue;
      }

      if (!curState->isSet(it->second)) {
        continue;
      }
      curState->unset(it->second);
      isAffected = isAffected || state->querier->isStmtType(StmtType::Assign,
                                                            stmtNumber);
    }

    if (isAffected) {
      state->querier->addAffectsCache(stmtNumber, state->writer->getLeft());
      if (!state->writer->writeLeft(stmtNumber)) {
        throw CFGHaltWalkerException();
      }
    }

    return;
  };

  CFGStatefulWalker<QueryToQueryState,
                    backwardWalkerCallback> statefulWalker(cfg, &queryState);
  statefulWalker.walkTo(nodeTo, initialState);

  if (type0 != StmtType::Wildcard) {
    querier.promoteAffectsTo(arg1);
  }
}

template<class T>
void CFGAffectsQuerier<T>::queryAll(StmtTransitiveResult *resultOut,
                                    const StmtType &type0,
                                    const StmtType &type1) {
  ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &querier, resultOut)
      .template makePairWriter<CFGAffectsQuerier::dummyTypePredicate>(0,
                                                                      type0,
                                                                      type1);

  for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
    StmtValue stmtNumber = cfg->fromCFGNode(start);
    writer->setLeft(stmtNumber);
    auto row = querier.queryAffectsFull(stmtNumber, 0);
    if (row != nullptr) {
      for (const StmtValue &i : *row) {
        writer->writeRight(i);
      }
      continue;
    }

    if (!validateArg(stmtNumber)) {
      continue;
    }

    queryForward(writer.get(), start);
    if (type1 != StmtType::Wildcard) {
      querier.promoteAffectsFrom(stmtNumber);
    }

    if (type0 == StmtType::Wildcard && type1 == StmtType::Wildcard
        && !resultOut->empty()) {
      break;
    }
  }
}

template<class T>
void CFGAffectsQuerier<T>::queryForward(ICFGWriter *writer,
                                        const CFGNode &start) {
  StmtValue stmtNumber = cfg->fromCFGNode(start);
  if (!validateArg(stmtNumber)) {
    return;
  }

  EntityIdxSet modifiedVars = querier.getModifies(stmtNumber);
  EntityIdx modifiedVar = SetUtils::firstItemOfSet(modifiedVars, NO_ENT_INDEX);
  QueryState queryState{writer, &querier, modifiedVar};

  constexpr WalkerSingleCallback<QueryState> forwardWalkerCallback
      = [](QueryState *queryState, CFGNode nextNode) -> bool {
        StmtValue stmtNumber = queryState->writer->toStmtNumber(nextNode);

        if (queryState->querier->isStmtType(StmtType::Assign, stmtNumber)) {
          EntityIdxSet usedVars = queryState->querier->getUses(stmtNumber);
          if (usedVars.find(queryState->target) != usedVars.end()) {
            queryState->querier->addAffectsCache(queryState->writer->getLeft(),
                                                 stmtNumber);
            if (!queryState->writer->writeRight(stmtNumber)) {
              throw CFGHaltWalkerException();
            }
          }
        }

        if (CFGAffectsQuerier::isContainer(queryState->querier, stmtNumber)) {
          return true;
        }

        return !SetUtils::setContains(
            queryState->querier->getModifies(stmtNumber),
            queryState->target);
      };
  walker.walkFrom<QueryState, forwardWalkerCallback>(start, &queryState);
}

template<class T>
bool CFGAffectsQuerier<T>::validateArg(const StmtValue &arg) const {
  return cfg->containsStatement(arg)
      && querier.isStmtType(StmtType::Assign, arg);
}
