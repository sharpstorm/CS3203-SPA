#include "SynonymConstraint.h"

SynonymConstraint::SynonymConstraint(const PQLSynonymName &syn1,
                                     const PQLSynonymName &syn2) :
    syn1(syn1), syn2(syn2) {}

bool SynonymConstraint::applyConstraint(SynonymProxyBuilder *synProxyBuilder,
                                        OverrideTable *overrideTable) {
  return synProxyBuilder->joinSynonyms(syn1, syn2);
}

PQLSynonymNameList SynonymConstraint::getAffectedSyns() const {
  return PQLSynonymNameList{syn1, syn2};
}

bool SynonymConstraint::validateConstraint() const {
  return true;
}
