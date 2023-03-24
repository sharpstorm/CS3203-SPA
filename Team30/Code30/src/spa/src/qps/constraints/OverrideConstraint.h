#pragma once

#include <string>
#include <vector>

#include "Constraint.h"
#include "qps/common/PQLQuerySynonym.h"
#include "OverrideTransformer.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/common/AttributedSynonym.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::string, std::to_string;

class OverrideConstraint : virtual public Constraint {
 private:
  AttributedSynonym attrSyn;
  OverrideTransformer overrideTransformer;

 public:
  OverrideConstraint(AttributedSynonym syn, string identVal);
  OverrideConstraint(AttributedSynonym syn, int intVal);

  bool applyConstraint(SynonymProxyBuilder* variableTable,
                       OverrideTable* overrideTable) override;
  vector<PQLSynonymName> getAffectedSyns() override;
  bool validateConstraint() override;
};
