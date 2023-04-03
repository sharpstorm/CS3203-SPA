#pragma once

#include "Constraint.h"
#include "qps/common/PQLQuerySynonym.h"
#include "OverrideTransformer.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/common/AttributedSynonym.h"

class OverrideConstraint : public Constraint {
 private:
  AttributedSynonym attrSyn;
  OverrideTransformer overrideTransformer;

 public:
  OverrideConstraint(const AttributedSynonym &syn, const EntityValue &identVal);
  OverrideConstraint(const AttributedSynonym &syn, const StmtValue &intVal);

  bool applyConstraint(SynonymProxyBuilder *variableTable,
                       OverrideTable *overrideTable) override;
  const PQLSynonymNameList getAffectedSyns() const override;
  bool validateConstraint() const override;
};
