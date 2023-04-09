#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "qps/common/synonym/AttributedSynonym.h"
#include "qps/common/IConstraint.h"
#include "qps/common/intermediate_result/QueryResultItemPool.h"

using std::unordered_map, std::unique_ptr, std::vector;

typedef int ProjectorResultRow;
typedef int ProjectorResultCol;
typedef vector<QueryResultItem *> ProjectorTableRow;
typedef vector<ProjectorTableRow> ProjectorTable;

class ProjectableGroup {
  ProjectorTable groupTable;
  unordered_map<PQLSynonymName, ProjectorResultCol> synIndex;
  QueryResultItemPool ownedItems;

  bool hasRowIn(const ProjectorTableRow &target,
                const ProjectableGroup &haystack) const;

 public:
  ProjectableGroup() = default;
  virtual ~ProjectableGroup() = default;
  void addRow(const ProjectorTableRow &row);
  void addSynonym(const PQLSynonymName &name);
  int getRowCount() const;
  const QueryResultItem *getEntryAt(const ProjectorResultRow row,
                                    const ProjectorResultCol col) const;
  QueryResultItemPool *getOwnedPool();
  ProjectorResultCol getSynonymCol(const PQLSynonymName &name) const;

  bool operator==(const ProjectableGroup &rg) const;

  static const ProjectorResultCol NO_COL = -1;
};

typedef unique_ptr<ProjectableGroup> ProjectableGroupPtr;
typedef vector<ProjectableGroupPtr> ProjectableGroupPtrList;
