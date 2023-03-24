#include "SynonymConstraint.h"

bool SynonymConstraint::applyConstraint
    (SynonymProxyBuilder* varTableProxyBuilder,
     OverrideTable* overrideTable) {
  varTableProxyBuilder->joinSynonyms(syn1, syn2);
  return true;
}

vector<PQLSynonymName> SynonymConstraint::getAffectedSyns() {
  return vector<PQLSynonymName>{syn1, syn2};
}

bool SynonymConstraint::validateConstraint() {
  return true;
}
