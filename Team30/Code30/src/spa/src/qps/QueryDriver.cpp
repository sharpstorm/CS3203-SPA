#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

QueryDriver::QueryDriver(PkbQueryHandler* pkbQH,
                         ISourceExpressionParser* exprParser):
    pkbQueryHandler(pkbQH),
    parser(new QueryParser(exprParser)),
    executor(new QueryExecutor(pkbQH)) { }

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
}

UniqueVectorPtr<string> QueryDriver::evaluate(string* query) {
  PQLQueryPtr pqlQuery = parser->parseQuery(query);
  SynonymResultTable* synTable = executor->executeQuery(pqlQuery.get());
  AttributedSynonymList* queryVar = pqlQuery->getResultVariables();
  UniqueVectorPtr<string> result = projector.project(
      synTable, queryVar, pkbQueryHandler);
  delete(synTable);
  return result;
}

