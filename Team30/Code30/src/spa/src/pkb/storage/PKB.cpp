#include "PKB.h"

#include <memory>

#include "tables/ContiguousTable.h"

using std::make_shared;

PKB::PKB()
    : followsStore(new FollowsStorage(make_shared<ContiguousTable<int>>(),
                                      make_shared<ContiguousTable<int>>())),
      parentStore(new ParentStorage(make_shared<ContiguousTable<int>>(),
                                    make_shared<ContiguousTable<int>>())),
      symbolStorage(new SymbolStorage()),
      statementStorage(new StatementStorage()),
      procedureStorage(new ProcedureStorage()),
      structureProvider(
          new StructureMappingProvider(statementStorage, procedureStorage)),
      entityMappingProvider(new EntityMappingProvider(symbolStorage)),
      predicateFactory(new PredicateFactory(structureProvider)) {}

PKB::~PKB() {
  delete(followsStore);
  delete(parentStore);
  delete(symbolStorage);
  delete(statementStorage);
  delete(procedureStorage);
  delete(structureProvider);
  delete(entityMappingProvider);
  delete(predicateFactory);
}
