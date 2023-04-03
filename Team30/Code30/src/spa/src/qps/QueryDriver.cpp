#include "QueryDriver.h"
#include "parser/QueryParser.h"
#include "executor/QueryExecutor.h"
#include "common/PQLQuerySynonym.h"

QueryDriver::QueryDriver(PkbQueryHandler* pkbQH,
                         ISourceExpressionParser* exprParser):
    parser(new QueryParser(exprParser)),
    executor(new QueryExecutor(pkbQH)),
    projector(new ResultProjector(pkbQH)) { }

QueryDriver::~QueryDriver() {
  delete(parser);
  delete(executor);
  delete(projector);
}

void QueryDriver::evaluate(const string &query,
                           QPSOutputList *output) {
  PQLQueryPtr pqlQuery = parser->parseQuery(&query);
  ProjectorResultTable* synTable = executor->executeQuery(pqlQuery.get());
  AttributedSynonymList* queryVar = pqlQuery->getResultVariables();
  projector->project(synTable, queryVar, output);
  delete(synTable);
}
