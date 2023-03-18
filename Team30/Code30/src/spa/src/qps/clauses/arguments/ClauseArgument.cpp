#include <string>

#include "ClauseArgument.h"

using std::to_string;

bool ClauseArgument::synonymSatisfies(SynonymPredicate predicate) {
  return true;
}

bool ClauseArgument::isNamed() {
  return false;
}

bool ClauseArgument::isWildcard() {
  return false;
}

PQLSynonymName ClauseArgument::getName() {
  return "";
}

PQLQuerySynonym *ClauseArgument::getSyn() {
  return nullptr;
}

bool ClauseArgument::isStatement(PQLQuerySynonym syn) {
  return syn.isStatementType();
}

bool ClauseArgument::canSubstitute(OverrideTable *table) {
  if (!isNamed()) {
    return false;
  }

  return table->find(getName()) != table->end();
}
StmtType ClauseArgument::getStmtTypeFromSynType(PQLSynonymType type) {
    switch (type) {
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
EntityType ClauseArgument::getEntTypeFromSynType(PQLSynonymType type) {
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
bool ClauseArgument::existsInPKB(PkbQueryHandler *handler,
                                 OverrideTransformer trans) {
  PQLQuerySynonym* syn = getSyn();

  if (syn == nullptr) {
    return false;
  }

  PQLSynonymType synType = syn->getType();

  // stmt/read/print/call/while/if/assign.stmt#
  if (syn->isStatementType() && trans.returnsInteger()) {
    StmtValue overrideVal = trans.getStmtValue();
    return handler->isStatementOfType(getStmtTypeFromSynType(synType),
                                      overrideVal);
  }

  // call.procName, read/print.varName
  EntityValue overrideVal = trans.getEntityValue();
  if (syn->isStatementType() && !trans.returnsInteger()) {
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
    overrideVal = to_string(trans.getStmtValue());
  }

  // procedure.procname, variable.varname, constant.value
  return handler->isSymbolOfType(getEntTypeFromSynType(synType),
                                 overrideVal);

}
