#include "OverrideConstraint.h"
#include "qps/errors/QPSParserSyntaxError.h"

OverrideConstraint::OverrideConstraint(const AttributedSynonym &syn,
                                       const StmtValue &intVal) :
    attrSyn(syn), overrideTransformer(OverrideTransformer(intVal)) {
}

OverrideConstraint::OverrideConstraint(const AttributedSynonym &syn,
                                       const EntityValue &identVal) :
    attrSyn(syn), overrideTransformer(OverrideTransformer(identVal)) {}

bool OverrideConstraint::validateConstraint() const {
  if (!attrSyn.hasAttribute()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_WITH_TYPE);
  }
  return attrSyn.returnsInteger() == overrideTransformer.returnsInteger();
}

PQLSynonymNameList OverrideConstraint::getAffectedSyns() const {
  return PQLSynonymNameList {attrSyn.getName()};
}

bool OverrideConstraint::applyConstraint(SynonymProxyBuilder *variableTable,
                                         OverrideTable *overrideTable) {
  const PQLSynonymName synName = attrSyn.getName();

  if (overrideTable->contains(synName)) {
    return overrideTable->get(synName) == overrideTransformer;
  }

  overrideTable->insert(synName, overrideTransformer);
  return true;
}
