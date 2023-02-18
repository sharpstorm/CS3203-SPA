#pragma once

#include "sp/extractor/AbstractExtractor.h"

class FollowsExtractor : public AbstractExtractor {
 public:
  explicit FollowsExtractor(PkbWriter* pkbWriter);
  void visit(StatementListNode* node);

 private:
  void addFollowsRelation(int x, int y);
  PkbWriter* pkbWriter;
};
