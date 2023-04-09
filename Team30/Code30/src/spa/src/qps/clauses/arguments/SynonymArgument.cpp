#include "SynonymArgument.h"
#include "qps/clauses/ClauseScoring.h"

SynonymArgument::SynonymArgument(const PQLQuerySynonymProxy &synProxy) :
    synProxy(synProxy) {
}

bool SynonymArgument::synonymSatisfies(SynonymPredicate predicate) const {
  return predicate(*synProxy);
}

bool SynonymArgument::isNamed() const {
  return true;
}

PQLSynonymName SynonymArgument::getName() const {
  return synProxy->getName();
}

ComplexityScore SynonymArgument::getSynComplexity() const {
  return COMPLEXITY_SYN_LOOKUP.at(synProxy->getType());
}

StmtRef SynonymArgument::toStmtRef() const {
  StmtType stmtType = convertToStmtType(synProxy->getType());
  return StmtRef{stmtType, NO_STMT};
}

EntityRef SynonymArgument::toEntityRef() const {
  EntityType entType = convertToEntityType(synProxy->getType());
  return EntityRef{entType, NO_ENT};
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
