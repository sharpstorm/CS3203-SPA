#include <utility>
#include <memory>

#include "QueryBuilder.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique;

QueryBuilder::QueryBuilder(): variables(make_unique<VariableTable>()) {
}

void QueryBuilder::setError(const ErrorMessage &msg) {
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

  variables->add(PQLQuerySynonym(type, name));
  declaredNames.insert(name);
}

bool QueryBuilder::hasSynonym(const PQLSynonymName &name) const {
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

void QueryBuilder::addSuchThat(SuchThatClausePtr clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addPattern(PatternClausePtr clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addWith(WithClausePtr clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addConstraint(ConstraintPtr constraint) {
  constraints.push_back(std::move(constraint));
}

void QueryBuilder::addWithSelect(WithSelectClausePtr clause) {
  clauses.push_back(std::move(clause));
}

PQLQueryPtr QueryBuilder::build() {
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
    if (!clauses.at(i)->validateArgTypes()) {
      throw QPSParserSemanticError(QPS_PARSER_ERR_SYNONYM_TYPE);
    }
  }

  return make_unique<PQLQuery>(std::move(variables), resultVariables,
                               std::move(clauses), std::move(constraints));
}
