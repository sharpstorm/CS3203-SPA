#include "PKB.h"

#include <memory>

#include "tables/ContiguousSetTable.h"
#include "tables/HashKeySetTable.h"
#include "tables/HashKeyTable.h"

using std::make_shared, std::string;

PKB::PKB() {}

PKB::~PKB() {
  delete (variableStorage);
  delete (variableTable);
  delete (variableRevTable);
  delete (constantStorage);
  delete (constantTable);
  delete (constantRevTable);
  delete (procedureStorage);
  delete (procedureTable);
  delete (procedureRevTable);
  delete (statementStorage);
  delete (stmtTable);
  delete (stmtRevTable);
  delete (callStmtStorage);
  delete (callStmtTable);
  delete (callStmtRevTable);

  delete (structureProvider);
  delete (entityMappingProvider);
  delete (stmtPredicateFactory);
  delete (entityPredicateFactory);

  delete (followsStorage);
  delete (followsTStorage);
  delete (followsTable);
  delete (followsRevTable);

  delete (parentStorage);
  delete (parentTStorage);
  delete (parentTable);
  delete (parentRevTable);

  delete (modifiesStorage);
  delete (modifiesTable);
  delete (modifiesRevTable);
  delete (modifiesPStorage);
  delete (modifiesPTable);
  delete (modifiesPRevTable);

  delete (usesStorage);
  delete (usesTable);
  delete (usesRevTable);
  delete (usesPStorage);
  delete (usesPTable);
  delete (usesPRevTable);

  delete (callsStorage);
  delete (callsTStorage);
  delete (callsTable);
  delete (callsRevTable);

  delete (ifPatternStorage);
  delete (ifPatternTable);
  delete (ifPatternRevTable);

  delete (whilePatternStorage);
  delete (whilePatternTable);
  delete (whilePatternRevTable);

  delete (assignStorage);
  delete (cfgStorage);
}
