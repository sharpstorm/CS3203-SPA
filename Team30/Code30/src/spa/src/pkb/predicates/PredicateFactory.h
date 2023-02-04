#pragma once

#include "../../common/Types.h"
#include "../storage/StructureMappingProvider.h"
#include "IsStmtOfTypePredicate.h"

class PredicateFactory {
 private:
  const StructureMappingProvider* provider;

 public:
  explicit PredicateFactory(const StructureMappingProvider* p);
  IsStmtOfTypePredicate getIsStmtOfTypePredicate(StmtType stmtType) const;
};
