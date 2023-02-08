#include "QueryBuilder.h"
#include "QueryBuilderError.h"

using std::move;

QueryBuilder::QueryBuilder() {
}

void QueryBuilder::setResultVariable(PQL_VAR_NAME name) {
  resultVariable.name = name;
}

void QueryBuilder::setResultType(PQLSynonymType type) {
  resultVariable.type = type;
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

PQLSynonymType* QueryBuilder::getVariableType(PQL_VAR_NAME name) {
  PQLQueryVariable* var = getVariable(name);
  if (var == nullptr) {
    return nullptr;
  }

  return &(var->type);
}

PQLQueryVariable* QueryBuilder::getVariable(PQL_VAR_NAME name) {
  if (!hasVariable(name)) {
    return nullptr;
  }

  return &variables[name];
}

void QueryBuilder::addSuchThat(unique_ptr<SuchThatClause> clause) {
  clauses.push_back(move(clause));
}

unique_ptr<PQLQuery> QueryBuilder::build() {
  for (int i = 0; i < clauses.size(); i++) {
    if (!clauses.at(i)->validateArgTypes(&variables)) {
      throw QueryBuilderError("Semantic Error, Invalid typing");
    }
  }

  unique_ptr<PQLQuery> created(new PQLQuery(variables,
                                            resultVariable, clauses));
  return created;
}