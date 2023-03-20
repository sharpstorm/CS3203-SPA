#pragma once

#include "Constraint.h"
#include <vector>

class SynonymConstraint : virtual public Constraint {
 private:
  PQLSynonymName syn1;
  PQLSynonymName syn2;
 public:
  SynonymConstraint(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2) :
      syn1(syn1), syn2(syn2) {}
  bool applyConstraint(SynonymProxyBuilder* varTableProxyBuilder,
                       OverrideTable* overrideTable);
  vector<PQLSynonymName> getAffectedSyns() override;
  bool validateConstraint() {
    return true;
  }
};
