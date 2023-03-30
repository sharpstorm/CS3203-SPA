#include "PKB.h"

PKB::PKB() {}

PKB::~PKB() {
  delete (variableStorage);
  delete (variableTable);
  delete (variableRevTable);
  delete (variableValues);
  delete (constantStorage);
  delete (constantTable);
  delete (constantRevTable);
  delete (constantValues);
  delete (procedureStmtTable);
  delete (stmtTable);
  delete (stmtRevTable);
  delete (callDeclarationTable);
  delete (procAndCallsStorage);

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
  delete (parentTTable);
  delete (parentTRevTable);

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
  delete (callsTTable);
  delete (callsTRevTable);

  delete (ifPatternStorage);
  delete (ifPatternTable);
  delete (ifPatternRevTable);

  delete (whilePatternStorage);
  delete (whilePatternTable);
  delete (whilePatternRevTable);

  delete (assignStorage);
  delete (cfgStorage);
}
