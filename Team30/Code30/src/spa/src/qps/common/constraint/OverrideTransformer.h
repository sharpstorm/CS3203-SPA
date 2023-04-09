#pragma once

#include "qps/common/synonym/PQLQuerySynonym.h"
#include "common/Types.h"

class OverrideTransformer {
 private:
  IntegerValue intValue;
  EntityValue entValue;

 public:
  explicit OverrideTransformer(const IntegerValue &intVal);
  explicit OverrideTransformer(const EntityValue &entVal);

  bool returnsInteger() const;
  StmtRef transformArg(const StmtRef &input) const;
  EntityRef transformArg(const EntityRef &input) const;

  bool operator==(const OverrideTransformer &other) const;
};
