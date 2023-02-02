#include <iostream>
#include "QueryBuilder.h"
#include "QueryBuilderError.h"

using std::cout;

QueryBuilder::QueryBuilder() {
}

void QueryBuilder::setResultVariable(PQL_VAR_NAME name) {
}

void QueryBuilder::setResultType(PQLSynonymType type) {
}

void QueryBuilder::addVariable(PQL_VAR_NAME name, PQLSynonymType type) {
  if (hasVariable(name)) {
    throw QueryBuilderError("Found duplicate variable");
  }
  variables[name] = (PQLQueryVariable{type, name});
}

bool QueryBuilder::hasVariable(PQL_VAR_NAME name) {
  return variables.find(name) != variables.end();
}

PQLSynonymType* QueryBuilder::getVariable(PQL_VAR_NAME name) {
  if (!hasVariable(name)) {
    return nullptr;
  }

  return &variables[name].type;
}

void QueryBuilder::addSuchThat(SuchThatClause* clause) {
}

unique_ptr<PQLQuery> QueryBuilder::build() {
  unique_ptr<PQLQuery> created(new PQLQuery(variables,
                                            resultVariable, clauses));
  return created;
}
