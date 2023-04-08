#pragma once

#include "qps/common/IConstraint.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/constraint/OverrideTransformer.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/common/AttributedSynonym.h"

class OverrideConstraint : public IConstraint {
 private:
  AttributedSynonym attrSyn;
  OverrideTransformer overrideTransformer;

 public:
  OverrideConstraint(const AttributedSynonym &syn, const EntityValue &identVal);
  OverrideConstraint(const AttributedSynonym &syn, const StmtValue &intVal);

  bool applyConstraint(SynonymProxyBuilder *variableTable,
                       OverrideTable *overrideTable) override;
  PQLSynonymNameList getAffectedSyns() const override;
  bool validateConstraint() const override;
};
