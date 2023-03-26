#include "SynonymConstraint.h"

bool SynonymConstraint::applyConstraint
    (SynonymProxyBuilder* varTableProxyBuilder,
     OverrideTable* overrideTable) {
  return varTableProxyBuilder->joinSynonyms(syn1, syn2);
}

vector<PQLSynonymName> SynonymConstraint::getAffectedSyns() {
  return vector<PQLSynonymName>{syn1, syn2};
}

bool SynonymConstraint::validateConstraint() {
  return true;
}
