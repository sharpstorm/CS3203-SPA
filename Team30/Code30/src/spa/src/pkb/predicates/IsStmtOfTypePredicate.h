#pragma once

#include "../storage/StructureMappingProvider.h"
#include "Predicate.h"

class IsStmtOfTypePredicate : public Predicate<int> {
 private:
  const StructureMappingProvider* provider;
  StmtType stmtType;

 public:
  IsStmtOfTypePredicate(const StructureMappingProvider* p, StmtType stmtType);
  bool operator()(int stmt) const;
};
