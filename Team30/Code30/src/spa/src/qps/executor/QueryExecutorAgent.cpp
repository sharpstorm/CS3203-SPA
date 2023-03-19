#include "QueryExecutorAgent.h"

QueryExecutorAgent::QueryExecutorAgent(PkbQueryHandler *pkb,
                                       OverrideTable *table):
    pkbQueryHandler(pkb), overrideTable(table) {}

PkbQueryHandler *QueryExecutorAgent::operator->() const {
  return this->pkbQueryHandler;
}

StmtRef QueryExecutorAgent::transform(const PQLSynonymName name,
                                      const StmtRef &ref) const {
  if (name.empty()) {
    return ref;
  }

  return overrideTable->transform(name, ref);
}

EntityRef QueryExecutorAgent::transform(const PQLSynonymName name,
                                        const EntityRef &ref) const {
  if (name.empty()) {
    return ref;
  }

  return overrideTable->transform(name, ref);
}

bool QueryExecutorAgent::isValid(const StmtRef &ref) const {
  if (ref.type == StmtType::None) {
    return true;
  }

  return !ref.isKnown()
      || pkbQueryHandler->isStatementOfType(ref.type, ref.lineNum);
}

bool QueryExecutorAgent::isValid(const EntityRef &ref) const {
  if (ref.type == EntityType::None) {
    return true;
  }

  return !ref.isKnown() ||
      pkbQueryHandler->isSymbolOfType(ref.type, ref.name);
}
