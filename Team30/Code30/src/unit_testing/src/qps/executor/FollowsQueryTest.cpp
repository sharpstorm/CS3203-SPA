#include "catch.hpp"

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/common/resulttable/SynonymResultTable.h"
#include "qps/executor/QueryExecutor.h"
#include "qps/parser/builder/QueryBuilder.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "../PKBStub.cpp"
#include "qps/clauses/such_that/FollowsClauses.h"

#include <memory>

using std::unordered_set, std::string, std::make_unique;

TEST_CASE("Test QPS Follows Query") {
  PKB pkbStore;
  auto pkb = make_unique<StubPKB>(&pkbStore);
  auto builder = QueryBuilder();
  AttributedSynonym syn = AttributedSynonym(PQLQuerySynonym(PQL_SYN_TYPE_STMT, "a"));
  builder.addResultSynonym(syn);
  builder.addSynonym("a", PQL_SYN_TYPE_STMT);
  builder.addSynonym("b", PQL_SYN_TYPE_STMT);
  builder.addSuchThat(make_unique<FollowsClause>(
      ClauseArgumentFactory::create(*builder.accessSynonym("a")),
      ClauseArgumentFactory::create(*builder.accessSynonym("b"))
  ));

  auto query = builder.build();

  auto executor = unique_ptr<IQueryExecutor>(new QueryExecutor(pkb.get()));
  SynonymResultTable* result = executor->executeQuery(query.get());
  delete(result);
}
