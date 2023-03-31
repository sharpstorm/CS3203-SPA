#include <memory>

#include "QueryExecutorAgent.h"

using std::make_unique;

QueryExecutorAgent::QueryExecutorAgent(PkbQueryHandler *pkb,
                                       OverrideTable *table):
    pkbQueryHandler(pkb), overrideTable(table) {}

PkbQueryHandler *QueryExecutorAgent::operator->() const {
  return this->pkbQueryHandler;
}

StmtRef QueryExecutorAgent::transformArg(const PQLSynonymName name,
                                         const StmtRef &ref) const {
  if (name.empty()) {
    return ref;
  }

  return overrideTable->transformArg(name, ref);
}

EntityRef QueryExecutorAgent::transformArg(const PQLSynonymName name,
                                           const EntityRef &ref) const {
  if (name.empty()) {
    return ref;
  }

  return overrideTable->transformArg(name, ref);
}

bool QueryExecutorAgent::isValid(const StmtRef &ref) const {
  if (ref.isType(StmtType::None)) {
    return true;
  }

  return !ref.isKnown()
      || pkbQueryHandler->isStatementOfType(ref.getType(), ref.getValue());
}

bool QueryExecutorAgent::isValid(const EntityRef &ref) const {
  if (ref.isType(EntityType::None)) {
    return true;
  }

  return !ref.isKnown() ||
      pkbQueryHandler->isSymbolOfType(ref.getType(), ref.getValue());
}

StmtStmtQueryResultPtr QueryExecutorAgent::toQueryResult(const StmtRef& left,
                                                         const StmtRef& right,
                                                         CacheRow *row) {
  StmtStmtQueryResultPtr result = make_unique<StmtStmtQueryResult>();

  if (row == nullptr || row->empty()) {
    return result;
  }

  // (static, static)
  if (left.isKnown() && right.isKnown()) {
    result->add(left.getValue(), right.getValue());
    return result;
  }

  // (syn, static)
  if (!left.isKnown()) {
    for (auto r : *row) {
      result->add(r, right.getValue());
    }
  }

  // (static, syn)
  if (!right.isKnown()) {
    for (auto r : *row) {
      result->add(left.getValue(), r);
    }
  }

  return result;
}

CacheTable QueryExecutorAgent::getNextTCache() {
  return nextTCache;
}

CacheTable QueryExecutorAgent::getAffectsCache() {
  return affectsCache;
}
