#pragma once

#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"
#include "qps/common/PQLQuerySynonymProxy.h"

class SynonymArgument: public ClauseArgument {
 private:
  PQLQuerySynonymProxy synProxy;

 public:
  explicit SynonymArgument(const PQLQuerySynonymProxy &synonym);
  bool synonymSatisfies(SynonymPredicate predicate) override;
  bool isNamed() override;
  PQLQuerySynonym* getSyn() override;
  PQLSynonymName getName() override;
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
};
