#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/interfaces/IIfPatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

using std::string;

class IfPatternQueryHandler : public IIfPatternQueryHandler {
 public:
  explicit IfPatternQueryHandler(const IfPatternStorage *store,
                                 const PredicateFactory *predicateFactory,
                                 const IStructureMappingProvider *provider);

  QueryResult<int, string> queryIfPattern(StmtRef, EntityRef) const override;

 private:
  const IfPatternStorage *store;
  const PredicateFactory *predicateFactory;
  const IStructureMappingProvider *structureProvider;

  bool validateArg1(StmtRef) const;
  bool validateArg2(EntityRef) const;
};
