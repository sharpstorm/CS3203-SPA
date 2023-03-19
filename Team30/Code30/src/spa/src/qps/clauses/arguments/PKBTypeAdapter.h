#pragma once

#include "common/Types.h"
#include "qps/common/PQLTypes.h"
#include "qps/errors/QPSError.h"
#include "qps/errors/QPSExecutionError.h"
class PKBTypeAdapter {
 public:
  static StmtType convertPQLSynonymToStmt(PQLSynonymType type) {
    StmtType returnType;
    switch (type) {
      case PQL_SYN_TYPE_CALL:
        returnType = StmtType::Call;
        break;
      case PQL_SYN_TYPE_ASSIGN:
        returnType = StmtType::Assign;
        break;
      case PQL_SYN_TYPE_IF:
        returnType = StmtType::If;
        break;
      case PQL_SYN_TYPE_WHILE:
        returnType = StmtType::While;
        break;
      case PQL_SYN_TYPE_READ:
        returnType = StmtType::Read;
        break;
      case PQL_SYN_TYPE_PRINT:
        returnType = StmtType::Print;
        break;
      case PQL_SYN_TYPE_STMT:
        returnType = StmtType::None;
        break;
      default:
        throw QPSExecutionError("asdfasdfasdf");
    }
    return returnType;
  }

  static EntityType convertPQLSynonymToEntity(PQLSynonymType type) {
    EntityType returnType;
    switch (type) {
      case PQL_SYN_TYPE_PROCEDURE:
        returnType = EntityType::Procedure;
        break;
      case PQL_SYN_TYPE_VARIABLE:
        returnType = EntityType::Variable;
        break;
      case PQL_SYN_TYPE_CONSTANT:
        returnType = EntityType::Constant;
        break;
      default:
        throw QPSExecutionError("asdfasdfasdf");
    }
    return returnType;
  }
};
