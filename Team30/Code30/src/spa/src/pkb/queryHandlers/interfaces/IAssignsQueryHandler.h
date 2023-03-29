#pragma once
#include <memory>

using std::unique_ptr;

class IAssignsQueryHandler {
 public:
  virtual ~IAssignsQueryHandler() {}
  virtual QueryResultPtr<StmtValue, PatternTrie *> queryAssigns(
      StmtRef) const = 0;
};
