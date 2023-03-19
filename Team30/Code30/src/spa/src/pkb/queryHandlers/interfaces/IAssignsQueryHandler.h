#pragma once

class IAssignsQueryHandler {
 public:
  virtual ~IAssignsQueryHandler() {}
  virtual QueryResult<StmtValue, PatternTrie *> queryAssigns(
      StmtRef) const = 0;
};
