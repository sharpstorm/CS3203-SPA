#pragma once

#include "../storage/StructureMappingProvider.h"
#include "Predicate.h"

class IsStmtOfTypePredicate : public RelPredicate<int> {
 private:
  const StructureMappingProvider* provider;
  StmtType stmtType;

 public:
  IsStmtOfTypePredicate(const StructureMappingProvider* p, StmtType stmtType);
  bool operator()(int const& stmt) const override;
};
