#include "SynonymConstraint.h"

bool SynonymConstraint::applyConstraint
    (SynonymProxyBuilder* varTableProxyBuilder,
     OverrideTable* overrideTable) {
  varTableProxyBuilder->joinSynonyms(syn1, syn2);
  return true;
}
