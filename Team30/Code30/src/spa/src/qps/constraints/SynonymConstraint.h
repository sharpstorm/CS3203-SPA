#pragma once

#include "qps/common/IConstraint.h"

class SynonymConstraint : public IConstraint {
 private:
  PQLSynonymName syn1;
  PQLSynonymName syn2;
 public:
  SynonymConstraint(const PQLSynonymName &syn1, const PQLSynonymName &syn2);
  bool applyConstraint(SynonymProxyBuilder *varTableProxyBuilder,
                       OverrideTable *overrideTable) override;
  PQLSynonymNameList getAffectedSyns() const override;
  bool validateConstraint() const override;
};
