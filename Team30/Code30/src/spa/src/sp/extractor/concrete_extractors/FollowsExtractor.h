#pragma once

#include "sp/extractor/AbstractExtractor.h"

class FollowsExtractor : public AbstractExtractor {
 public:
  explicit FollowsExtractor(PkbWriter* pkbWriter);
  void visitStmtList(StatementListNode* node) override;

 private:
  void addFollowsRelation(LineNumber x, LineNumber y);
  PkbWriter* pkbWriter;
};
