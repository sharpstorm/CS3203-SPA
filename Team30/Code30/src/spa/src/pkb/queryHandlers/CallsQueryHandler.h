#pragma once

#include <string>

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/StorageTypes.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "interfaces/ICallsQueryHandler.h"

using std::string;

class CallsQueryHandler : public ICallsQueryHandler {
 public:
  CallsQueryHandler(const CallsStorage *store,
                    const PredicateFactory *predicateFactory,
                    const IEntityMappingProvider *entityProvider);

  QueryResult<string, string> queryCalls(EntityRef e1,
                                         EntityRef e2) const override;
  QueryResult<string, string> queryCallsStar(EntityRef e1,
                                             EntityRef e2) const override;

 private:
  const CallsStorage *store;
  const PredicateFactory *predicateFactory;
  const IEntityMappingProvider *entityProvider;
  bool validateArg1(EntityRef) const;
  bool validateArg2(EntityRef) const;
};
