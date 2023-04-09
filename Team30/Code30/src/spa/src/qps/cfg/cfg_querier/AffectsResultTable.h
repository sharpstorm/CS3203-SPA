#pragma once

#include "common/Types.h"
#include "common/cfg/CFG.h"
#include "common/data_structs/BitField.h"
#include "CFGQuerier.h"

typedef QueryResultSet<StmtValue> AffectsResult;
typedef vector<AffectsResult> AffectsResultList;

template<class AffectsQuerier>
class AffectsResultTable {
  AffectsResultList affectsResults;
  BitField queried;
  CFG *cfg;
  AffectsQuerier *querier;

 public:
  AffectsResultTable(CFG *cfg, AffectsQuerier *querier, int size)
      : cfg(cfg), querier(querier), affectsResults(size), queried(size) {}

  const AffectsResult &getResultAt(const StmtValue stmt) {
    CFGNode nodeId = cfg->toCFGNode(stmt);
    if (!queried.isSet(nodeId)) {
      StmtTransitiveResult result = querier->queryFrom(stmt, StmtType::Assign);
      affectsResults[nodeId] = result.getRightVals();
      queried.set(nodeId);
    }

    return affectsResults.at(nodeId);
  }
};
