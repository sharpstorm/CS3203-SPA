#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/AttributedSynonym.h"
#include "qps/constraints/Constraint.h"

using std::unordered_map, std::unique_ptr, std::vector;

typedef int ProjectorResultRow;
typedef int ProjectorResultCol;

class ProjectorResultGroup {
  QueryResultTable groupTable;
  unordered_map<PQLSynonymName, ResultTableCol> colMap;
  PQLSynonymNameList colIdx;
  QueryResultItemPool ownedItems;

  bool hasRowIn(const QueryResultTableRow &target,
                const ProjectorResultGroup &haystack) const;

 public:
  ProjectorResultGroup() = default;
  virtual ~ProjectorResultGroup() = default;
  void addRow(const QueryResultTableRow &row);
  void addSynonym(const PQLSynonymName &name);
  int getRowCount() const;
  const QueryResultTableRow *getRowAt(const ProjectorResultRow &idx) const;
  QueryResultItemPool *getOwnedPool();
  ResultTableCol getSynonymCol(const PQLSynonymName &name) const;

  bool operator==(const ProjectorResultGroup &rg) const;

  static const ResultTableCol NO_COL = -1;
};

typedef unique_ptr<ProjectorResultGroup> ResultGroupPtr;
