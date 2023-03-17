#pragma once

#include <string>
#include "Constraint.h"
#include "qps/common/PQLQuerySynonym.h"
#include "OverrideTransformer.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::string, std::to_string;

class OverrideConstraint : virtual public Constraint {
 private:
  AttributedSynonym syn;
  OverrideTransformer overrideTransformer;

 public:
  OverrideConstraint(AttributedSynonym syn, string identVal) :
      syn(syn), overrideTransformer(OverrideTransformer(identVal)) { }
  OverrideConstraint(AttributedSynonym syn, int intVal) :
      syn(syn), overrideTransformer(OverrideTransformer(intVal)) { }

  bool applyConstraint(VariableTable* varTable,
                       OverrideTable* overrideTable,
                       PkbQueryHandler* handler) {
    PQLSynonymName synName = syn.getName();
    PQLSynonymType synType = syn.getType();

    if (overrideTable->find(synName) != overrideTable->end()) {
      return false;
    }

    if (!existInPKB(handler, synType)) {
      return false;
    }



    overrideTable->insert({synName, overrideTransformer});
    return true;
  }

  bool validateConstraint() {
    if (!syn.validateAttribute()) {
      return false;
    }

    switch (syn.getAttribute()) {
      case STMT_NUM:
      case CONST_VALUE:
        return overrideTransformer.returnsInteger();
      case PROC_NAME:
      case VAR_NAME:
        return !overrideTransformer.returnsInteger();
      default:
        throw QPSParserSemanticError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    }
  }

  StmtType getStmtTypeFromSynType(PQLSynonymType synType) {
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

  EntityType getEntTypeFromSynType(PQLSynonymType synType) {
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

  bool existInPKB(PkbQueryHandler* handler, PQLSynonymType synType) {
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
};
