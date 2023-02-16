#include "QueryBuilder.h"
#include "QueryBuilderError.h"

using std::move, std::make_unique;

QueryBuilder::QueryBuilder() {
}

void QueryBuilder::setResultVariable(PQLSynonymType type, PQLSynonymName name) {
  resultVariable = PQLQuerySynonym(type, name);
}

void QueryBuilder::addVariable(PQLSynonymName name, PQLSynonymType type) {
  if (hasVariable(name)) {
    throw QueryBuilderError("Found duplicate variable");
  }
  variables[name] = PQLQuerySynonym(type, name);
}

bool QueryBuilder::hasVariable(PQLSynonymName name) {
  return variables.find(name) != variables.end();
}

PQLQuerySynonym* QueryBuilder::getVariable(PQLSynonymName name) {
  if (!hasVariable(name)) {
    return nullptr;
  }

  return &variables[name];
}

void QueryBuilder::addSuchThat(unique_ptr<SuchThatClause> clause) {
  clauses.push_back(move(clause));
}

void QueryBuilder::addPattern(unique_ptr<PatternClause> clause) {
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
