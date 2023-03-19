#pragma once

#include "Constraint.h"

class SynonymConstraint : virtual public Constraint {
 private:
  PQLSynonymName syn1;
  PQLSynonymName syn2;
 public:
  SynonymConstraint(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2) :
      syn1(syn1), syn2(syn2) {}
  bool applyConstraint(VariableTableProxyBuilder* varTableProxyBuilder,
                       OverrideTable* overrideTable);
  bool validateConstraint() {
    return true;
  }
};
