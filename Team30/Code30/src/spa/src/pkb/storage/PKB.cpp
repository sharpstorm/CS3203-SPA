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
      modifiesStorage(new ModifiesStorage(
          make_shared<HashKeySetTable<int, string>>(),
          make_shared<HashKeySetTable<string, int>>())),
      usesStorage(new UsesStorage(
          make_shared<HashKeySetTable<int, string>>(),
          make_shared<HashKeySetTable<string, int>>())),
      symbolStorage(new SymbolStorage(
          make_shared<HashKeyTable<std::string, EntityType>>(),
          make_shared<HashKeySetTable<EntityType, std::string>>())),
      statementStorage(
          new StatementStorage(make_shared<ContiguousTable<StmtType>>(),
                               make_shared<HashKeySetTable<StmtType, int>>())),
      procedureStorage(new ProcedureStorage(
          make_shared<ContiguousTable<std::string>>(),
          make_shared<HashKeySetTable<std::string, int>>())),
      structureProvider(
          new StructureMappingProvider(statementStorage, procedureStorage)),
      entityMappingProvider(new EntityMappingProvider(symbolStorage)),
      predicateFactory(
          new PredicateFactory(structureProvider, entityMappingProvider)) {}
PKB::~PKB() {
  delete (followsStore);
  delete (parentStore);
  delete (modifiesStorage);
  delete (usesStorage);
  delete (symbolStorage);
  delete (statementStorage);
  delete (procedureStorage);
  delete (structureProvider);
  delete (entityMappingProvider);
  delete (predicateFactory);
}
