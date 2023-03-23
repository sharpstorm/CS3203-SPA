#include "OverrideConstraint.h"

OverrideConstraint::OverrideConstraint(AttributedSynonym syn,
                                       int intVal):
    attrSyn(syn), overrideTransformer(OverrideTransformer(intVal)) {
}

OverrideConstraint::OverrideConstraint(AttributedSynonym syn,
                                       string identVal):
    attrSyn(syn), overrideTransformer(OverrideTransformer(identVal)) {}

bool OverrideConstraint::validateConstraint() {
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

vector<PQLSynonymName> OverrideConstraint::getAffectedSyns() {
  return vector<PQLSynonymName>{attrSyn.getName()};
}

bool OverrideConstraint::applyConstraint(SynonymProxyBuilder *variableTable,
                                         OverrideTable *overrideTable) {
  PQLSynonymName synName = attrSyn.getName();

  if (overrideTable->contains(synName)) {
    return false;
  }

  overrideTable->insert(synName, overrideTransformer);
  return true;
}
