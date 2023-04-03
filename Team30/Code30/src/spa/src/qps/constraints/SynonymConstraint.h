#pragma once

#include "Constraint.h"

class SynonymConstraint : public Constraint {
 private:
  PQLSynonymName syn1;
  PQLSynonymName syn2;
 public:
  SynonymConstraint(const PQLSynonymName &syn1, const PQLSynonymName &syn2);
  bool applyConstraint(SynonymProxyBuilder *varTableProxyBuilder,
                       OverrideTable *overrideTable) override;
  const PQLSynonymNameList getAffectedSyns() const override;
  bool validateConstraint() const override;
};
