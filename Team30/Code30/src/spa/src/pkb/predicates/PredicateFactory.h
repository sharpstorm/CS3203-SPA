#pragma once

#include "../../common/Types.h"
#include "../storage/StructureMappingProvider.h"
#include "Predicate.h"

class PredicateFactory {
 private:
  const StructureMappingProvider* provider;

 public:
  explicit PredicateFactory(const StructureMappingProvider* p);

  Predicate<int> getPredicate(StmtRef stmtRef) const;
};
