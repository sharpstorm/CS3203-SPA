#include <utility>
#include <memory>

#include "QueryBuilder.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique;

QueryBuilder::QueryBuilder() {
  variables = make_unique<VariableTable>();
}

void QueryBuilder::setError(const string &msg) {
  if (!errorMsg.empty()) {
    return;
  }
  errorMsg = msg;
}

void QueryBuilder::addResultSynonym(const AttributedSynonym &synonym) {
  resultVariables.push_back(synonym);
}

void QueryBuilder::addSynonym(const PQLSynonymName &name,
                              const PQLSynonymType &type) {
  if (hasSynonym(name)) {
    setError(QPS_PARSER_ERR_DUPLICATE_SYN);
    return;
  }

  variables->emplace(name, PQLQuerySynonym(type, name));
}

bool QueryBuilder::hasSynonym(const PQLSynonymName &name) {
  return variables->find(name) != variables->end();
}

PQLQuerySynonym* QueryBuilder::accessSynonym(const PQLSynonymName &name) {
  if (!hasSynonym(name)) {
    setError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    return nullptr;
  }

  return &(variables->at(name));
}

void QueryBuilder::addSuchThat(unique_ptr<SuchThatClause> clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addPattern(unique_ptr<PatternClause> clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addWith(unique_ptr<WithClause> clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addConstraint(ConstraintSPtr constraint) {
  constraints.push_back(constraint);
}

unique_ptr<PQLQuery> QueryBuilder::build() {
  if (!errorMsg.empty()) {
    throw QPSParserSemanticError(errorMsg);
  }

  // Constraint Validation
  for (int i = 0; i < constraints.size(); i++) {
    if (!constraints.at(i)->validateConstraint()) {
      throw QPSParserSemanticError(QPS_PARSER_ERR_SYNONYM_TYPE);
    }
  }

  // Clause Validation
  for (int i = 0; i < clauses.size(); i++) {
    if (!clauses.at(i)->validateArgTypes(variables.get())) {
      throw QPSParserSemanticError(QPS_PARSER_ERR_SYNONYM_TYPE);
    }
  }

  unique_ptr<PQLQuery> created(
      new PQLQuery(
          std::move(variables), resultVariables, clauses, constraints));
  return created;
}
