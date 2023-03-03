#include "catch.hpp"

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/executor/SynonymResultTable.h"
#include "qps/executor/QueryExecutor.h"
#include "qps/parser/builder/QueryBuilder.h"
#include "qps/clauses/such_that/FollowsClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "../PKBStub.cpp"

#include <memory>

using std::unordered_set, std::string, std::make_unique;

TEST_CASE("Test QPS Follows Query") {
  PKB pkbStore;
  PkbQueryHandler* pkb = new StubPKB(&pkbStore);
  auto builder = QueryBuilder();
  builder.addResultSynonym(PQLQuerySynonym(PQL_SYN_TYPE_STMT, "a"));
  builder.addSynonym("a", PQL_SYN_TYPE_STMT);
  builder.addSynonym("b", PQL_SYN_TYPE_STMT);
  builder.addSuchThat(make_unique<FollowsClause>(
      ClauseArgumentFactory::create(*builder.accessSynonym("a")),
      ClauseArgumentFactory::create(*builder.accessSynonym("b"))
  ));

  auto query = builder.build();

  auto handler = shared_ptr<PkbQueryHandler>(pkb);
  auto executor = unique_ptr<IQueryExecutor>(
      new QueryExecutor(handler));
  SynonymResultTable* result = executor->executeQuery(query.get());
  delete(result);
}
