#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "qps/common/AttributedSynonym.h"
#include "qps/common/IConstraint.h"
#include "qps/common/intermediate_result/QueryResultItemPool.h"

using std::unordered_map, std::unique_ptr, std::vector;

typedef int ProjectorResultRow;
typedef int ProjectorResultCol;
typedef vector<QueryResultItem *> ProjectorTableRow;
typedef vector<ProjectorTableRow> ProjectorTable;

class ProjectorResultGroup {
  ProjectorTable groupTable;
  unordered_map<PQLSynonymName, ProjectorResultCol> synIndex;
  QueryResultItemPool ownedItems;

  bool hasRowIn(const ProjectorTableRow &target,
                const ProjectorResultGroup &haystack) const;

 public:
  ProjectorResultGroup() = default;
  virtual ~ProjectorResultGroup() = default;
  void addRow(const ProjectorTableRow &row);
  void addSynonym(const PQLSynonymName &name);
  int getRowCount() const;
  const QueryResultItem *getEntryAt(const ProjectorResultRow row,
                                    const ProjectorResultCol col) const;
  QueryResultItemPool *getOwnedPool();
  ProjectorResultCol getSynonymCol(const PQLSynonymName &name) const;

  bool operator==(const ProjectorResultGroup &rg) const;

  static const ProjectorResultCol NO_COL = -1;
};

typedef unique_ptr<ProjectorResultGroup> ResultGroupPtr;
