#include <utility>
#include <memory>
#include <unordered_set>

#include "QueryBuilder.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique;

QueryBuilder::QueryBuilder(): variables(make_unique<VariableTable>()) {
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

  variables->add(name, PQLQuerySynonym(type, name));
  declaredNames.insert(name);
}

bool QueryBuilder::hasSynonym(const PQLSynonymName &name) {
  return declaredNames.find(name) != declaredNames.end();
}

void QueryBuilder::finalizeSynonymTable() {
  variables->finalizeTable();
}

PQLQuerySynonymProxy* QueryBuilder::accessSynonym(const PQLSynonymName &name) {
  if (!hasSynonym(name)) {
    setError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    return nullptr;
  }

  return (variables->find(name));
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

void QueryBuilder::addWithSelect(unique_ptr<WithSelectClause> clause) {
  clauses.push_back(std::move(clause));
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
