#pragma once

#include "common/Types.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"
#include "AbstractPredicateFactory.h"

class StmtPredicateFactory : public AbstractPredicateFactory<StmtValue,
                                                             StmtType> {
 public:
  StmtPredicateFactory(IStructureMappingProvider *provider);

  Predicate<StmtValue> handleType(IRef<StmtValue,
                                       StmtType> *ref) const override;

 private:
  IStructureMappingProvider *provider;
};
