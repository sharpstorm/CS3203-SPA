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
//StmtType ClauseArgument::getStmtTypeFromSynType(PQLSynonymType type) {
//    switch (type) {
//      case PQL_SYN_TYPE_READ:
//        return StmtType::Read;
//      case PQL_SYN_TYPE_PRINT:
//        return StmtType::Print;
//      case PQL_SYN_TYPE_CALL:
//        return StmtType::Call;
//      case PQL_SYN_TYPE_WHILE:
//        return StmtType::While;
//      case PQL_SYN_TYPE_IF:
//        return StmtType::If;
//      case PQL_SYN_TYPE_ASSIGN:
//        return StmtType::Assign;
//      default:
//        return StmtType::None;
//    }
//}
//EntityType ClauseArgument::getEntTypeFromSynType(PQLSynonymType type) {
//    switch (type) {
//      case PQL_SYN_TYPE_PROCEDURE:
//        return EntityType::Procedure;
//      case PQL_SYN_TYPE_VARIABLE:
//        return EntityType::Variable;
//      case PQL_SYN_TYPE_CONSTANT:
//        return EntityType::Constant;
//      default:
//        return EntityType::None;
//    }
//}
