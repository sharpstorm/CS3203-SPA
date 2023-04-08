#include "catch.hpp"

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/executor/IQueryExecutor.h"
#include "qps/common/projector_table/ProjectableTable.h"
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
  auto querySyn = make_unique<PQLQuerySynonym>(PQLQuerySynonym(PQL_SYN_TYPE_STMT, "a"));
  auto synPtr = querySyn.get();
  PQLQuerySynonymProxy proxy(&synPtr);
  AttributedSynonym syn = AttributedSynonym(proxy);
  builder.addResultSynonym(syn);
  builder.addSynonym("a", PQL_SYN_TYPE_STMT);
  builder.addSynonym("b", PQL_SYN_TYPE_STMT);
  builder.finalizeSynonymTable();
  builder.addSuchThat(make_unique<FollowsClause>(
      ClauseArgumentFactory::create(*builder.accessSynonym("a")),
      ClauseArgumentFactory::create(*builder.accessSynonym("b"))
  ));

  auto query = builder.build();

  auto executor = unique_ptr<IQueryExecutor>(new QueryExecutor(pkb.get()));
  ProjectableTable* result = executor->executeQuery(query.get());
  delete(result);
}
