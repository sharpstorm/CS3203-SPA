#include "QueryBuilder.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/errors/QPSParserSyntaxError.h"

using std::make_unique;

QueryBuilder::QueryBuilder(): errorMsg("") {
}

void QueryBuilder::setError(string msg) {
  if (errorMsg != "") {
    return;
  }
  errorMsg = msg;
}

void QueryBuilder::addResultSynonym(PQLQuerySynonym synonym) {
  resultVariables.push_back(synonym);
}

void QueryBuilder::addSynonym(PQLSynonymName name, PQLSynonymType type) {
  if (hasSynonym(name)) {
    setError(QPS_PARSER_ERR_DUPLICATE_SYN);
    return;
  }
  variables[name] = PQLQuerySynonym(type, name);
}

bool QueryBuilder::hasSynonym(PQLSynonymName name) {
  return variables.find(name) != variables.end();
}

PQLQuerySynonym* QueryBuilder::accessSynonym(PQLSynonymName name) {
  if (!hasSynonym(name)) {
    setError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    return nullptr;
  }

  return &variables[name];
}

void QueryBuilder::addSuchThat(unique_ptr<SuchThatClause> clause) {
  clauses.push_back(std::move(clause));
}

void QueryBuilder::addPattern(unique_ptr<PatternClause> clause) {
  clauses.push_back(std::move(clause));
}

unique_ptr<PQLQuery> QueryBuilder::build() {
  if (errorMsg != "") {
    throw QPSParserSemanticError(errorMsg.c_str());
  }

  for (int i = 0; i < clauses.size(); i++) {
    if (!clauses.at(i)->validateArgTypes(&variables)) {
      throw QPSParserSemanticError(QPS_PARSER_ERR_SYNONYM_TYPE);
    }
  }

  unique_ptr<PQLQuery> created(
      new PQLQuery(variables, resultVariables, clauses));
  return created;
}
