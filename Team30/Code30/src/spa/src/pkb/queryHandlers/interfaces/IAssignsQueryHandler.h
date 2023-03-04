#pragma once

#include <memory>

class IAssignsQueryHandler {
 public:
  virtual ~IAssignsQueryHandler() {}
  virtual QueryResult<int, PatternTrie*> queryAssigns(
      StmtRef) const = 0;
};
