#pragma once

#include "Predicate.h"
#include <string>
#include "../../common/Types.h"
#include "../storage/StructureMappingProvider.h"
#include "../storage/EntityMappingProvider.h"

using std::string;

class PredicateFactory {
 private:
  const IStructureMappingProvider *structureProvider;
  const IEntityMappingProvider *entityProvider;

 public:
  explicit PredicateFactory(const IStructureMappingProvider *,
                            const IEntityMappingProvider *);

  Predicate<int> getPredicate(StmtRef) const;
  Predicate<string> getPredicate(EntityRef) const;
  Predicate<int> getIsStmtOfProcedurePredicate(string) const;
};
