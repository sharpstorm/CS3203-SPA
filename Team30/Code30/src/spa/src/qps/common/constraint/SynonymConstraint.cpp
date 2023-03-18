#include "SynonymConstraint.h"

bool SynonymConstraint::applyConstraint
    (VariableTableProxyBuilder* varTableProxyBuilder, OverrideTable* overrideTable) {
  varTableProxyBuilder->joinSynonyms(syn1, syn2);
  return true;
}
