#include "SynonymArgument.h"
#include "PKBTypeAdapter.h"

SynonymArgument::SynonymArgument(const PQLQuerySynonymProxy &synProxy):
    synProxy(synProxy) {
}

bool SynonymArgument::synonymSatisfies(SynonymPredicate predicate) {
  return predicate(*synProxy);
}

bool SynonymArgument::isNamed() {
  return true;
}

PQLSynonymName SynonymArgument::getName() {
  return synProxy->getName();
}

StmtRef SynonymArgument::toStmtRef() {
  StmtType stmtType =
      PKBTypeAdapter::convertPQLSynonymToStmt(synProxy->getType());
  return StmtRef{stmtType, 0};
}

EntityRef SynonymArgument::toEntityRef() {
  EntityType entType =
      PKBTypeAdapter::convertPQLSynonymToEntity(synProxy->getType());
  return EntityRef{ entType, "" };
}

PQLQuerySynonym *SynonymArgument::getSyn() {
  return synProxy.get();
}
