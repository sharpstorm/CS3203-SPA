#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/AttributedSynonym.h"
#include "qps/constraints/Constraint.h"

using std::unordered_map, std::unique_ptr, std::vector;

class ProjectorResultGroup {
  QueryResultTable groupTable;
  unordered_map<PQLSynonymName, ResultTableCol> colMap;
  vector<PQLSynonymName> colIdx;
  QueryResultItemPool ownedItems;

 public:
  ProjectorResultGroup() = default;
  virtual ~ProjectorResultGroup() = default;
  void addRow(QueryResultTableRow row);
  void addSynonym(PQLSynonymName name);
  int getRowCount() const;
  const QueryResultTableRow* getRowAt(int idx) const;
  QueryResultItemPool* getOwnedPool();
  ResultTableCol getSynonymCol(const PQLSynonymName &name) const;

  bool operator ==(const ProjectorResultGroup &rg) const;

  static const ResultTableCol NO_COL = -1;
};

typedef unique_ptr<ProjectorResultGroup> ResultGroupPtr;
