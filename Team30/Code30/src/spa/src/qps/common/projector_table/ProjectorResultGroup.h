#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/AttributedSynonym.h"
#include "qps/constraints/Constraint.h"

using std::unordered_map, std::unique_ptr, std::string, std::vector;

class ProjectorResultGroup {
  QueryResultTable groupTable;
  unordered_map<PQLSynonymName, ResultTableCol> colMap;
  vector<PQLSynonymName> colIdx;
  QueryResultItemPool ownedItems;

  static string projectNonDefaultAttribute(PkbQueryHandler* handler,
                                           QueryResultItem* item,
                                           AttributedSynonym syn);
  static bool isNonDefaultCase(AttributedSynonym syn);

 public:
  ProjectorResultGroup() = default;
  virtual ~ProjectorResultGroup() = default;
  void addRow(QueryResultTableRow row);
  void addSynonym(PQLSynonymName name);
  void addColMap(vector<PQLSynonymName> map);
  int getTableRows();
  QueryResultTableRow* getRowAt(int idx);
  vector<PQLSynonymName>* getColIndexes();
  ProjectorResultGroup* crossProduct(ProjectorResultGroup* other);
  void project(AttributedSynonymList* synList,
               PkbQueryHandler* handler,
               QPSOutputList* output);
  QueryResultItemPool* getOwnedPool();

  bool operator ==(const ProjectorResultGroup &rg) const;
};

typedef unique_ptr<ProjectorResultGroup> ResultGroupPtr;
