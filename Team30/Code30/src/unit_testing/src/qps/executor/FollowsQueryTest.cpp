#include "catch.hpp"

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/QueryDriver.h"
#include "qps/executor/QueryExecutor.h"
#include "qps/parser/builder/QueryBuilder.h"
#include "qps/clauses/FollowsClause.h"
#include "../PKBStub.cpp"

#include <memory>

using std::unordered_set, std::string, std::make_unique;

TEST_CASE("Test QPS Follows Query") {
  PKB pkbStore;
  PkbQueryHandler* pkb = new StubPKB(&pkbStore);
  auto builder = QueryBuilder();
  builder.setResultVariable("a");
  builder.setResultType(PQL_VAR_TYPE_STMT);
  builder.addVariable("a", PQL_VAR_TYPE_STMT);
  builder.addVariable("b", PQL_VAR_TYPE_STMT);
  builder.addSuchThat(make_unique<FollowsClause>(
      ClauseArgument(*builder.getVariable("a")),
      ClauseArgument(*builder.getVariable("b"))
  ));

  auto query = builder.build();

  auto handler = shared_ptr<PkbQueryHandler>(pkb);
  auto executor = unique_ptr<IQueryExecutor>(
      new QueryExecutor(handler));
  PQLQueryResult* result = executor->executeQuery(query.get());
  delete(result);
}
