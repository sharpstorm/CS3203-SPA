#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IWhilePatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

using std::string;

class WhilePatternQueryHandler : public IWhilePatternQueryHandler {
 public:
  explicit WhilePatternQueryHandler(const WhilePatternStorage *store,
                                    const PredicateFactory *predicateFactory,
                                    const IStructureMappingProvider *provider);

  QueryResult<int, string> queryWhilePattern(StmtRef, EntityRef) const override;

 private:
  const WhilePatternStorage *store;

  const PredicateFactory *predicateFactory;
  const IStructureMappingProvider *structureProvider;

  bool validateArg1(StmtRef) const;
  bool validateArg2(EntityRef) const;
};
