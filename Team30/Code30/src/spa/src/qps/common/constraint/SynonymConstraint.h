#pragma once

#include "Constraint.h"

class SynonymConstraint : virtual public Constraint {
 private:
  PQLQuerySynonym* syn1;
  PQLQuerySynonym* syn2;
 public:
  SynonymConstraint(PQLQuerySynonym* syn1, PQLQuerySynonym* syn2) :
      syn1(syn1), syn2(syn2) {}
  bool applyConstraint(VariableTableProxyBuilder* varTableProxyBuilder,
                       OverrideTable* overrideTable);
  bool validateConstraint() {
    return true;
  }
};
