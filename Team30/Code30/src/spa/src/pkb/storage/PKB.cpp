#include "PKB.h"

#include <memory>

#include "tables/ContiguousSetTable.h"
#include "tables/HashKeySetTable.h"
#include "tables/HashKeyTable.h"

using std::make_shared;

PKB::PKB()
    : followsStore(new FollowsStorage(make_shared<ContiguousSetTable<int>>(),
                                      make_shared<ContiguousSetTable<int>>())),
      parentStore(new ParentStorage(make_shared<ContiguousSetTable<int>>(),
                                    make_shared<ContiguousSetTable<int>>())),
      symbolStorage(new SymbolStorage(
          make_shared<HashKeyTable<std::string, EntityType>>(),
          make_shared<HashKeySetTable<EntityType, std::string>>())),

      statementStorage(
          new StatementStorage(make_shared<ContiguousTable<StmtType>>(),
                               make_shared<HashKeySetTable<StmtType, int>>())),

      procedureStorage(new ProcedureStorage()),
      structureProvider(
          new StructureMappingProvider(statementStorage, procedureStorage)),
      entityMappingProvider(new EntityMappingProvider(symbolStorage)),
      predicateFactory(
          new PredicateFactory(structureProvider, entityMappingProvider)) {}
PKB::~PKB() {
  delete (followsStore);
  delete (parentStore);
  delete (symbolStorage);
  delete (statementStorage);
  delete (procedureStorage);
  delete (structureProvider);
  delete (entityMappingProvider);
  delete (predicateFactory);
}
