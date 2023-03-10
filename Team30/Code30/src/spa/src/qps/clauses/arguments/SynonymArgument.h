#pragma once

#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"

class SynonymArgument: public ClauseArgument {
 private:
  PQLQuerySynonym synonym;

 public:
  explicit SynonymArgument(const PQLQuerySynonym &synonym);
  bool synonymSatisfies(SynonymPredicate predicate) override;
  bool isNamed() override;
  PQLSynonymName getName() override;
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
};
