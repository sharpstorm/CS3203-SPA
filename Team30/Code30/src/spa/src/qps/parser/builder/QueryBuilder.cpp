#include <iostream>
#include "QueryBuilder.h"

using std::cout;

QueryBuilder::QueryBuilder() {
}

void QueryBuilder::setResultVariable(PQL_VAR_NAME name) {
}

void QueryBuilder::setResultType(PQL_VAR_TYPE type) {
}

void QueryBuilder::addVariable(PQL_VAR_NAME name, PQL_VAR_TYPE type) {
  variables.push_back(QueryVariable{type, name});
}

void QueryBuilder::addSuchThat(SuchThatClause* clause) {
}

unique_ptr<PQLQuery> QueryBuilder::build() {
  unique_ptr<PQLQuery> created(new PQLQuery(variables, resultVariable, clauses));
  return created;
}
