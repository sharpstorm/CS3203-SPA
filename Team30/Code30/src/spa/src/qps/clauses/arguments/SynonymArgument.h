#pragma once

#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"
#include "qps/common/PQLQuerySynonymProxy.h"

class SynonymArgument: public ClauseArgument {
 private:
  PQLQuerySynonymProxy synProxy;

  static EntityType convertToEntityType(PQLSynonymType type);
  static StmtType convertToStmtType(PQLSynonymType type);

 public:
  explicit SynonymArgument(const PQLQuerySynonymProxy &synonym);
  bool synonymSatisfies(SynonymPredicate predicate) override;
  bool isNamed() override;
  PQLSynonymName getName() override;
  ComplexityScore getSynComplexity() override;
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
};
