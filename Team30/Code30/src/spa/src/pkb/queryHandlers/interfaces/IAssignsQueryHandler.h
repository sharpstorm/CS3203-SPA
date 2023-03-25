#pragma once
#include <memory>

using std::unique_ptr;

class IAssignsQueryHandler {
 public:
  virtual ~IAssignsQueryHandler() {}
  virtual unique_ptr<QueryResult<StmtValue, PatternTrie *>> queryAssigns(
      StmtRef) const = 0;
};
