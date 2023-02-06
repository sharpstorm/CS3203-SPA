#pragma once

#include "sp/extractor/AbstractExtractor.h"

class FollowsExtractor : public AbstractExtractor {
 public:
  FollowsExtractor() = default;
  virtual void visit(StatementListNode node);
 private:
  void addFollowsRelation(int x, int y);
};
