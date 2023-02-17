#include "ClauseArgument.h"

ClauseArgument::ClauseArgument(ClauseArgumentType type): argType(type) {
}

ClauseArgument::ClauseArgument(PQLQuerySynonym variable):
    argType(CLAUSE_ARG_SYNONYM),
    synonym(variable) {
}

ClauseArgument::ClauseArgument(int stmt):
    argType(CLAUSE_ARG_INTEGER), statement(stmt) {
}

ClauseArgument::ClauseArgument(string ident):
    argType(CLAUSE_ARG_STRING), ident(ident) {
}

PQLSynonymType ClauseArgument::getSynonymType() {
  return this->synonym.getType();
}

PQLSynonymName ClauseArgument::getSynonymName() {
  return this->synonym.getName();
}

bool ClauseArgument::isWildcard() {
  return this->argType == CLAUSE_ARG_WILDCARD;
}

bool ClauseArgument::isSynonym() {
  return this->argType == CLAUSE_ARG_SYNONYM;
}

bool ClauseArgument::isStmtRef() {
  return this->argType == CLAUSE_ARG_INTEGER;
}

bool ClauseArgument::isEntRef() {
  return this->argType == CLAUSE_ARG_STRING;
}

bool ClauseArgument::isValid() {
  return this->argType != CLAUSE_ARG_INVALID;
}

int ClauseArgument::getStatement() {
  return statement;
}

string ClauseArgument::getIdent() {
  return ident;
}
