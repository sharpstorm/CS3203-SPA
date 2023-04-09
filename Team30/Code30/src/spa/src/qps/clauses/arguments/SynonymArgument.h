#pragma once

#include "ClauseArgument.h"
#include "qps/common/synonym/PQLQuerySynonym.h"
#include "qps/common/synonym/PQLQuerySynonymProxy.h"

class SynonymArgument : public ClauseArgument {
 private:
  PQLQuerySynonymProxy synProxy;

  static EntityType convertToEntityType(PQLSynonymType type);
  static StmtType convertToStmtType(PQLSynonymType type);

 public:
  explicit SynonymArgument(const PQLQuerySynonymProxy &synonym);
  bool synonymSatisfies(SynonymPredicate predicate) const override;
  bool isNamed() const override;
  PQLSynonymName getName() const override;
  ComplexityScore getSynComplexity() const override;
  StmtRef toStmtRef() const override;
  EntityRef toEntityRef() const override;
};
