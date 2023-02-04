#pragma once

#include "../storage/StructureMappingProvider.h"
#include "Predicate.h"

class IsStmtOfTypePredicate : public Predicate<int> {
 private:
  StructureMappingProvider* provider;
  StmtType stmtType;

 public:
  IsStmtOfTypePredicate(StructureMappingProvider* p, int x);
  bool operator()(int y);
};
