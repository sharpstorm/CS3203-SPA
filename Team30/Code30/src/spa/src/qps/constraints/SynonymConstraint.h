#pragma once

#include <vector>

#include "Constraint.h"

class SynonymConstraint : virtual public Constraint {
 private:
  PQLSynonymName syn1;
  PQLSynonymName syn2;
 public:
  SynonymConstraint(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2) :
      syn1(syn1), syn2(syn2) {}
  bool applyConstraint(SynonymProxyBuilder* varTableProxyBuilder,
                       OverrideTable* overrideTable) override;
  vector<PQLSynonymName> getAffectedSyns() override;
  bool validateConstraint() override;
};
