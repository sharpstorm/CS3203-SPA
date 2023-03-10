#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "qps/common/PQLQueryResult.h"
#include "qps/common/AttributedSynonym.h"

using std::unordered_map, std::unique_ptr, std::string, std::vector;

class ResultGroup {
  QueryResultTable groupTable;
  unordered_map<PQLSynonymName, ResultTableCol> colMap;
  vector<PQLSynonymName> colIdx;

 public:
  ResultGroup() = default;
  virtual ~ResultGroup() = default;
  void addRow(QueryResultTableRow row);
  void addSynonym(PQLSynonymName name);
  void addColMap(vector<PQLSynonymName> map);
  int getTableRows();
  QueryResultTableRow* getRowAt(int idx);
  vector<PQLSynonymName>* getColIndexes();
  ResultGroup* crossProduct(ResultGroup* other);
  void project(AttributedSynonymList* synList, vector<string>* result);
  bool operator ==(const ResultGroup &rg) const;
};

typedef unique_ptr<ResultGroup> ResultGroupPtr;
