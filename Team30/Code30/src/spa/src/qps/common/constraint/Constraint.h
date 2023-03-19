#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "qps/common/VariableTable.h"
#include "OverrideTransformer.h"
#include "SynonymProxyBuilder.h"

using std::string, std::shared_ptr, std::unordered_map, std::unique_ptr;

typedef unordered_map<string, OverrideTransformer> OverrideTable;
typedef unique_ptr<OverrideTable> OverrideTablePtr;

class Constraint {
 public:
  virtual ~Constraint() = default;
  virtual bool applyConstraint(SynonymProxyBuilder* variableTable,
                               OverrideTable* overrideTable) = 0;
  virtual bool validateConstraint() = 0;
};

typedef shared_ptr<Constraint> ConstraintSPtr;
