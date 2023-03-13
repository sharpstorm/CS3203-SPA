#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

UniqueVectorPtr<string> QueryDriver::evaluate(string* query) {
  PQLQueryPtr pqlQuery = parser->parseQuery(query);
  PQLQuerySynonym* testSyn = pqlQuery->getVariable("r");
  SynonymResultTable* synTable = executor->executeQuery(pqlQuery.get());
  AttributedSynonymList* queryVar = pqlQuery->getResultVariables();
  UniqueVectorPtr<string> result = projector.project(synTable, queryVar);
  delete(synTable);
  return result;
}

QueryDriver::QueryDriver(PkbQueryHandler* pkbQH,
                         ISourceExpressionParser* exprParser):
  parser(new QueryParser(exprParser)), executor(new QueryExecutor(pkbQH)) {
}

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
}
