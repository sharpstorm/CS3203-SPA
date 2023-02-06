#include <memory>

#include "QPSInitialiser.h"
#include "QPSFacade.h"
#include "qps/IQueryDriver.h"
#include "qps/QueryDriver.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/executor/QueryExecutor.h"
#include "qps/executor/QueryLauncher.h"
#include "qps/executor/QueryOrchestrator.h"
#include "qps/parser/IQueryParser.h"
#include "qps/parser/QueryParser.h"

using std::shared_ptr;

IQPS* QPSInitialiser::initQPS(shared_ptr<PkbQueryHandler> pkbQH) {
  // Execution side
  QueryLauncher launcher = QueryLauncher(pkbQH);
  QueryOrchestrator orchestrator = QueryOrchestrator(launcher);
  IQueryExecutor* executor = new QueryExecutor(orchestrator);

  // Parsing side
  IQueryParser* parser = new QueryParser();

  IQueryDriver* driver = new QueryDriver(parser, executor);
  IQPS* qps = new QPSFacade(driver);

  return qps;
}
