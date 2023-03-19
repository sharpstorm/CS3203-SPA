#pragma once

#include "common/Types.h"
#include "qps/common/PQLTypes.h"
#include "qps/errors/QPSError.h"
#include "qps/errors/QPSExecutionError.h"
class PKBTypeAdapter {
 public:
  static StmtType convertPQLSynonymToStmt(PQLSynonymType type) {
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
      case PQL_SYN_TYPE_STMT:
        return StmtType::None;
      default:
        throw QPSExecutionError("Execution Error");
    }
  }

  static EntityType convertPQLSynonymToEntity(PQLSynonymType type) {
    switch (type) {
      case PQL_SYN_TYPE_PROCEDURE:
        return EntityType::Procedure;
      case PQL_SYN_TYPE_VARIABLE:
        return EntityType::Variable;
      case PQL_SYN_TYPE_CONSTANT:
        return EntityType::Constant;
      default:
        throw QPSExecutionError("Execution Error");
    }
  }
};
