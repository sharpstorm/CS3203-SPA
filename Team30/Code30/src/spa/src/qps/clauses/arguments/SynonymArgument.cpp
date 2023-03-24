#include "SynonymArgument.h"

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
  StmtType stmtType = convertToStmtType(synProxy->getType());
  return StmtRef{stmtType, 0};
}

EntityRef SynonymArgument::toEntityRef() {
  EntityType entType = convertToEntityType(synProxy->getType());
  return EntityRef{ entType, "" };
}

EntityType SynonymArgument::convertToEntityType(PQLSynonymType type) {
  switch (type) {
    case PQL_SYN_TYPE_PROCEDURE:
      return EntityType::Procedure;
    case PQL_SYN_TYPE_VARIABLE:
      return EntityType::Variable;
    case PQL_SYN_TYPE_CONSTANT:
      return EntityType::Constant;
    default:
      return EntityType::None;
  }
}

StmtType SynonymArgument::convertToStmtType(PQLSynonymType type) {
  switch (type) {
    case PQL_SYN_TYPE_CALL:
      return StmtType::Call;
    case PQL_SYN_TYPE_ASSIGN:
      return StmtType::Assign;
    case PQL_SYN_TYPE_IF:
      return StmtType::If;
    case PQL_SYN_TYPE_WHILE:
      return StmtType::While;
    case PQL_SYN_TYPE_READ:
      return StmtType::Read;
    case PQL_SYN_TYPE_PRINT:
      return StmtType::Print;
    default:
      return StmtType::None;
  }
}
