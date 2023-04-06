#include "OverrideConstraint.h"

OverrideConstraint::OverrideConstraint(const AttributedSynonym &syn,
                                       const StmtValue &intVal) :
    attrSyn(syn), overrideTransformer(OverrideTransformer(intVal)) {
}

OverrideConstraint::OverrideConstraint(const AttributedSynonym &syn,
                                       const EntityValue &identVal) :
    attrSyn(syn), overrideTransformer(OverrideTransformer(identVal)) {}

bool OverrideConstraint::validateConstraint() const {
  switch (attrSyn.getAttribute()) {
    case STMT_NUM:
    case CONST_VALUE:
      return overrideTransformer.returnsInteger();
    case PROC_NAME:
    case VAR_NAME:
      return !overrideTransformer.returnsInteger();
    default:
      throw QPSParserSemanticError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
  }
}

const PQLSynonymNameList OverrideConstraint::getAffectedSyns() const {
  return PQLSynonymNameList {attrSyn.getName()};
}

bool OverrideConstraint::applyConstraint(SynonymProxyBuilder *variableTable,
                                         OverrideTable *overrideTable) {
  PQLSynonymName synName = attrSyn.getName();

  if (overrideTable->contains(synName)) {
    return overrideTable->get(synName) == overrideTransformer;
  }

  overrideTable->insert(synName, overrideTransformer);
  return true;
}
