#include "SynonymArgument.h"

SynonymArgument::SynonymArgument(PQLQuerySynonymProxy synProxy):
    synProxy(synProxy) {
}

bool SynonymArgument::synonymSatisfies(SynonymPredicate predicate) {
  return predicate(*resolveProxy());
}

bool SynonymArgument::isNamed() {
  return true;
}

PQLSynonymName SynonymArgument::getName() {
  return resolveProxy()->getName();
}

PQLQuerySynonym *SynonymArgument::resolveProxy() {
  return *synProxy;
}

StmtRef SynonymArgument::toStmtRef() {
  switch (resolveProxy()->getType()) {
    case PQL_SYN_TYPE_ASSIGN:
      return StmtRef{StmtType::Assign, 0};
    case PQL_SYN_TYPE_READ:
      return StmtRef{StmtType::Read, 0};
    case PQL_SYN_TYPE_CALL:
      return StmtRef{StmtType::Call, 0};
    case PQL_SYN_TYPE_WHILE:
      return StmtRef{StmtType::While, 0};
    case PQL_SYN_TYPE_IF:
      return StmtRef{StmtType::If, 0};
    case PQL_SYN_TYPE_PRINT:
      return StmtRef{StmtType::Print, 0};
    default:
      return StmtRef{StmtType::None, 0};
  }
}

EntityRef SynonymArgument::toEntityRef() {
  switch (resolveProxy()->getType()) {
    case PQL_SYN_TYPE_VARIABLE:
      return EntityRef{EntityType::Variable, ""};
    case PQL_SYN_TYPE_CONSTANT:
      return EntityRef{EntityType::Constant, ""};
    case PQL_SYN_TYPE_PROCEDURE:
      return EntityRef{EntityType::Procedure, ""};
    default:
      return EntityRef{EntityType::None};
  }
}

PQLQuerySynonym *SynonymArgument::getSyn() {
  return resolveProxy();
}
