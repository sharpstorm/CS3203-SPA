#pragma once

#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"

class SynonymArgument: public ClauseArgument {
 private:
  PQLQuerySynonymProxy synProxy;

  PQLQuerySynonym* resolveProxy();

 public:
  explicit SynonymArgument(PQLQuerySynonymProxy synonym);
  bool synonymSatisfies(SynonymPredicate predicate) override;
  bool isNamed() override;
  PQLQuerySynonym* getSyn() override;
  PQLSynonymName getName() override;
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
};
