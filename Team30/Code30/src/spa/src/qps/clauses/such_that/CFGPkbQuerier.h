#pragma once

#include <algorithm>

#include "qps/executor/QueryExecutorAgent.h"

class CFGPkbQuerier {
 private:
  const QueryExecutorAgent &agent;
  CacheTable *affectsCache;

 public:
  explicit CFGPkbQuerier(const QueryExecutorAgent &agent) :
      agent(agent), affectsCache(agent.getAffectsCache()) {}

  bool isStmtType(StmtType type, StmtValue value) const {
    if (type == StmtType::None || type == StmtType::Wildcard) {
      return true;
    }
    return agent->getStatementType(value) == type;
  }
  EntityIdxSet getModifies(StmtValue value) const {
    QueryResultPtr<StmtValue, EntityValue> result =
        agent->queryModifies(StmtRef{StmtType::None, value},
                             EntityRef{EntityType::Variable, ""});
    EntityIdxSet ret;
    for (const EntityValue &v : result->getRightVals()) {
      ret.insert(agent->getIndexOfVariable(v));
    }
    ret.erase(NO_ENT_INDEX);
    return ret;
  }

  EntityIdxSet getUses(StmtValue value) const {
    QueryResultPtr<StmtValue, EntityValue> result =
        agent->queryUses(StmtRef{StmtType::None, value},
                         EntityRef{EntityType::Variable, ""});
    EntityIdxSet ret;
    for (const EntityValue &v : result->getRightVals()) {
      ret.insert(agent->getIndexOfVariable(v));
    }
    ret.erase(NO_ENT_INDEX);
    return ret;
  }

  bool queryAffectsPartial(StmtValue arg0, StmtValue arg1) const {
    return affectsCache->queryPartial(arg0, arg1) != nullptr;
  }

  const CacheRow *queryAffectsFull(StmtValue arg0, StmtValue arg1) const {
    return affectsCache->queryFull(arg0, arg1);
  }

  void addAffectsCache(StmtValue arg0, StmtValue arg1) {
    affectsCache->addEntry(arg0, arg1);
  }

  void promoteAffectsFrom(StmtValue from) {
    affectsCache->promoteFrom(from);
  }
  void promoteAffectsTo(StmtValue to) {
    affectsCache->promoteTo(to);
  }
};
