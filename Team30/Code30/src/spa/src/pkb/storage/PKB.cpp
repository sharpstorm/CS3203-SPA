#include "PKB.h"

#include <memory>

#include "tables/ContiguousSetTable.h"
#include "tables/HashKeySetTable.h"
#include "tables/HashKeyTable.h"

using std::make_shared, std::string;

PKB::PKB()
    : followsStore(new FollowsStorage(make_shared<ContiguousSetTable<int>>(),
                                      make_shared<ContiguousSetTable<int>>())),
      parentStore(new ParentStorage(make_shared<ContiguousSetTable<int>>(),
                                    make_shared<ContiguousSetTable<int>>())),
      modifiesStorage(
          new ModifiesStorage(make_shared<HashKeySetTable<int, string>>(),
                              make_shared<HashKeySetTable<string, int>>())),
      modifiesPStorage(
          new ModifiesPStorage(make_shared<HashKeySetTable<string, string>>(),
                               make_shared<HashKeySetTable<string, string>>())),
      usesStorage(new UsesStorage(make_shared<HashKeySetTable<int, string>>(),
                                  make_shared<HashKeySetTable<string, int>>())),
      usesPStorage(new UsesPStorage(make_shared<HashKeySetTable<string,
                                                                string>>(),
                                    make_shared<HashKeySetTable<string,
                                                                string>>())),
      symbolStorage(new SymbolStorage()),
      statementStorage(
          new StatementStorage(make_shared<ContiguousTable<StmtType>>(),
                               make_shared<HashKeySetTable<StmtType, int>>())),
      assignStorage(new AssignStorage()),
      ifPatternStorage(new IfPatternStorage(
          make_shared<HashKeySetTable<int, string>>(),
          make_shared<HashKeySetTable<string, int>>())),
      whilePatternStorage(new WhilePatternStorage(
          make_shared<HashKeySetTable<string, int>>())),
      callStmtStorage(new CallStmtStorage(
          make_shared<HashKeyTable<int, string>>(),
          make_shared<HashKeySetTable<string, int>>())),
      callsStorage(new CallsStorage(
          make_shared<HashKeySetTable<string, string>>(),
          make_shared<HashKeySetTable<string, string>>())),
      procedureStorage(new ProcedureStorage(
          make_shared<ContiguousTable<string>>(),
          make_shared<HashKeySetTable<string, int>>())),
      structureProvider(
          new StructureMappingProvider(statementStorage,
                                       procedureStorage,
                                       callStmtStorage)),
      entityMappingProvider(new EntityMappingProvider(symbolStorage)),
      predicateFactory(
          new PredicateFactory(structureProvider, entityMappingProvider)) {}
PKB::~PKB() {
  delete (followsStore);
  delete (parentStore);
  delete (modifiesStorage);
  delete (modifiesPStorage);
  delete (usesStorage);
  delete (usesPStorage);
  delete (symbolStorage);
  delete (statementStorage);
  delete (procedureStorage);
  delete (assignStorage);
  delete (ifPatternStorage);
  delete (structureProvider);
  delete (entityMappingProvider);
  delete (predicateFactory);
  delete (callStmtStorage);
  delete (callsStorage);
}
