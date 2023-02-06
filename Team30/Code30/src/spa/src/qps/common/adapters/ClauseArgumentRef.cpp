#include "ClauseArgumentRef.h"

StmtRef ClauseArgumentRef::toStmtRef(ClauseArgument clauseArgument) {
  if (clauseArgument.isStmtRef()) {
    return StmtRef{StmtType::None, clauseArgument.getStatement()};
  }

  if (clauseArgument.isWildcard()) {
    return StmtRef{StmtType::None, 0};
  }

  PQLSynonymType synType = clauseArgument.getSynonymType();
  switch (synType) {
    case PQL_VAR_TYPE_ASSIGN:
      return StmtRef{StmtType::Assign, 0};
    case PQL_VAR_TYPE_READ:
      return StmtRef{StmtType::Read, 0};
    case PQL_VAR_TYPE_CALL:
      return StmtRef{StmtType::Call, 0};
    case PQL_VAR_TYPE_WHILE:
      return StmtRef{StmtType::While, 0};
    case PQL_VAR_TYPE_PRINT:
      return StmtRef{StmtType::Print, 0};
    default:
      return StmtRef{StmtType::None, 0};
  }
}

EntityRef ClauseArgumentRef::toEntityRef(ClauseArgument clauseArgument) {
  if (clauseArgument.isEntRef()) {
    return EntityRef{EntityType::None, clauseArgument.getIdent()};
  }

  if (clauseArgument.isWildcard()) {
    return EntityRef{EntityType::None, ""};
  }

  PQLSynonymType synType = clauseArgument.getSynonymType();
  switch (synType) {
    case PQL_VAR_TYPE_VARIABLE:
      return EntityRef{EntityType::Variable, ""};
    case PQL_VAR_TYPE_CONSTANT:
      return EntityRef{EntityType::Constant, ""};
    default:
      return EntityRef{EntityType::None};
  }
}
