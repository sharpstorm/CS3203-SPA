#include "catch.hpp"

#include <memory>

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "WithSelectPKBStub.cpp"
#include "qps/common/synonym/AttributedSynonym.h"
#include "../util/SynonymHolder.h"
#include "../util/QueryResultTestUtil.cpp"
#include "qps/clauses/WithSelectClause.h"

using std::make_unique, std::unique_ptr;

typedef unordered_map<PQLSynonymName, PQLSynonymAttribute> SynAttrMap;

SynonymHolder WITH_SELECT_SYNS({
                                   {PQL_SYN_TYPE_CALL, "cl"},
                                   {PQL_SYN_TYPE_PRINT, "pn"},
                                   {PQL_SYN_TYPE_READ, "r"}
                               });

SynAttrMap WITH_SELECT_ATTR({
                                {"cl", PROC_NAME},
                                {"pn", VAR_NAME},
                                {"r", VAR_NAME}
                            });

TEST_CASE("WithSelectClause - evaluateOn - no transform") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<WithSelectPKBStub>(&pkb);
  OverrideTable overrideTable;
  QueryCache overrideCache;
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  vector<PQLSynonymName> syns = vector<PQLSynonymName>({"r", "pn", "cl"});

  for (size_t i = 0; i < syns.size(); i++) {
    PQLSynonymName synName = syns[i];
    AttributedSynonym syn(WITH_SELECT_SYNS.getProxy(synName), WITH_SELECT_ATTR[synName]);
    EntityValue entVal = "x";
    WithSelectClause clause = WithSelectClause(syn, entVal);

    auto expected = TestQueryResultBuilder::buildExpected(
        ExpectedParams{
            {synName, QueryResultItemVector{
                TestResultItem(i+1),
            }}
        }
    );

    auto actual = clause.evaluateOn(agent);
    REQUIRE(*expected.get() == *actual);
    delete actual;
  }
}

TEST_CASE("WithSelectClause - evaluateOn - transform (valid sub)") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<WithSelectPKBStub>(&pkb);
  OverrideTable overrideTable;
  overrideTable.insert("r", OverrideTransformer(1));
  overrideTable.insert("pn", OverrideTransformer(2));
  overrideTable.insert("cl", OverrideTransformer(3));
  QueryCache overrideCache;
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  vector<PQLSynonymName> syns = vector<PQLSynonymName>({"r", "pn", "cl"});

  for (size_t i = 0; i < syns.size(); i++) {
    PQLSynonymName synName = syns[i];
    AttributedSynonym syn(WITH_SELECT_SYNS.getProxy(synName), WITH_SELECT_ATTR[synName]);
    EntityValue entVal = "x";
    WithSelectClause clause = WithSelectClause(syn, entVal);

    auto expected = TestQueryResultBuilder::buildExpected(
        ExpectedParams{
            {synName, QueryResultItemVector{
                TestResultItem(i+1),
            }}
        }
    );

    auto actual = clause.evaluateOn(agent);
    INFO(synName);
    REQUIRE(*expected.get() == *actual);
    delete actual;
  }
}

TEST_CASE("WithSelectClause - evaluateOn - transform (invalid sub)") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<WithSelectPKBStub>(&pkb);
  OverrideTable overrideTable;
  overrideTable.insert("r", OverrideTransformer(3));
  overrideTable.insert("pn", OverrideTransformer(8));
  overrideTable.insert("cl", OverrideTransformer(5));
  QueryCache overrideCache;
  QueryExecutorAgent agent(handler.get(), &overrideTable, &overrideCache);
  vector<PQLSynonymName> syns = vector<PQLSynonymName>({"r", "pn", "cl"});

  PQLQueryResult expected;
  for (size_t i = 0; i < syns.size(); i++) {
    PQLSynonymName synName = syns[i];
    AttributedSynonym syn(WITH_SELECT_SYNS.getProxy(synName), WITH_SELECT_ATTR[synName]);
    EntityValue entVal = "x";
    WithSelectClause clause = WithSelectClause(syn, entVal);

    auto actual = clause.evaluateOn(agent);
    REQUIRE(expected == *actual);
    delete actual;
  }
}
