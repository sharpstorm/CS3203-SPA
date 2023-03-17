#include "OverrideConstraint.h"

StmtType OverrideConstraint::getStmtTypeFromSynType(PQLSynonymType synType) {
    switch (synType) {
      case PQL_SYN_TYPE_READ:
        return StmtType::Read;
      case PQL_SYN_TYPE_PRINT:
        return StmtType::Print;
      case PQL_SYN_TYPE_CALL:
        return StmtType::Call;
      case PQL_SYN_TYPE_WHILE:
        return StmtType::While;
      case PQL_SYN_TYPE_IF:
        return StmtType::If;
      case PQL_SYN_TYPE_ASSIGN:
        return StmtType::Assign;
      default:
        return StmtType::None;
    }
}

EntityType OverrideConstraint::getEntTypeFromSynType(PQLSynonymType synType) {
    switch (synType) {
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
bool OverrideConstraint::existsInPKB(PkbQueryHandler *handler,
                                     PQLSynonymType synType) {
  // stmt/read/print/call/while/if/assign.stmt#
  if (syn.isStatementType() && syn.returnsInteger()) {
    StmtValue overrideVal = overrideTransformer.getStmtValue();
    return handler->isStatementOfType(getStmtTypeFromSynType(synType),
                                      overrideVal);
  }

  // call.procName, read/print.varName
  EntityValue overrideVal = overrideTransformer.getEntityValue();
  if (syn.isStatementType() && !syn.returnsInteger()) {
    StmtType sType = getStmtTypeFromSynType(synType);

    for (auto s : handler->getStatementsOfType(sType)) {
      if (synType == PQL_SYN_TYPE_READ
          && handler->getReadDeclarations(s) == overrideVal) {
        return true;
      } else if (synType == PQL_SYN_TYPE_PRINT
          && handler->getPrintDeclarations(s) == overrideVal) {
        return true;
      } else if (handler->getCalledDeclaration(s) == overrideVal) {
        return true;
      }
    }
    return false;
  }

  if (synType == PQL_SYN_TYPE_CONSTANT) {
    overrideVal = to_string(overrideTransformer.getStmtValue());
  }

  // procedure.procname, variable.varname, constant.value
  return handler->isSymbolOfType(getEntTypeFromSynType(synType),
                                 overrideVal);
}
