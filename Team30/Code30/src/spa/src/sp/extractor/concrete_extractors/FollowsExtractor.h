#pragma once

#include "sp/extractor/AbstractExtractor.h"

class FollowsExtractor : public AbstractExtractor {
 public:
  FollowsExtractor(PkbWriter* pkbWriter);
  virtual void visit(StatementListNode node);
 private:
  void addFollowsRelation(int x, int y);
  PkbWriter* pkbWriter;
};
