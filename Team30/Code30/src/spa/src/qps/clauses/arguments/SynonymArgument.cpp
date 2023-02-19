#include "SynonymArgument.h"

SynonymArgument::SynonymArgument(PQLQuerySynonym synonym): synonym(synonym) {
}

bool SynonymArgument::synonymSatisfies(SynonymPredicate predicate) {
  return predicate(synonym);
}

bool SynonymArgument::isNamed() {
  return true;
}

PQLSynonymName SynonymArgument::getName() {
  return synonym.getName();
}

StmtRef SynonymArgument::toStmtRef() {
  switch (synonym.getType()) {
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
  switch (synonym.getType()) {
    case PQL_SYN_TYPE_VARIABLE:
      return EntityRef{EntityType::Variable, ""};
    case PQL_SYN_TYPE_CONSTANT:
      return EntityRef{EntityType::Constant, ""};
    default:
      return EntityRef{EntityType::None};
  }
}
