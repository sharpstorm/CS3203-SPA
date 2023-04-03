#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "common/Types.h"

class OverrideTransformer {
 private:
  StmtValue stmtRef;
  EntityValue entRef;

 public:
  explicit OverrideTransformer(const StmtValue &stmt);
  explicit OverrideTransformer(const EntityValue &ent);

  bool returnsInteger() const;
  StmtRef transformArg(const StmtRef &input) const;
  EntityRef transformArg(const EntityRef &input) const;

  bool operator==(const OverrideTransformer &other) const;
};
