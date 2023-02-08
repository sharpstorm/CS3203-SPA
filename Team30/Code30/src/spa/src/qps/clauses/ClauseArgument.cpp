#include "ClauseArgument.h"

ClauseArgument::ClauseArgument(ClauseArgumentType type) {
  this->argType = type;
}

ClauseArgument::ClauseArgument(PQLQueryVariable variable) {
  this->argType = CLAUSE_ARG_SYNONYM;
  this->synonym = variable;
}

ClauseArgument::ClauseArgument(int stmt) {
  this->argType = CLAUSE_ARG_INTEGER;
  this->statement = stmt;
}

ClauseArgument::ClauseArgument(string ident) {
  this->argType = CLAUSE_ARG_STRING;
  this->ident = ident;
}

PQLSynonymType ClauseArgument::getSynonymType() {
  return this->synonym.type;
}

PQL_VAR_NAME ClauseArgument::getSynonymName() {
  return this->synonym.name;
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

int ClauseArgument::getStatement() {
  return statement;
}

string ClauseArgument::getIdent() {
  return ident;
}