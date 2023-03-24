#include "catch.hpp"

#include "SourceParserStub.h"
#include "qps/IQPS.h"
#include "QPSQueryPKBStub.cpp"
#include "qps/QueryDriver.h"

TEST_CASE("QPS Execute Query") {
  PKB pkbStore;
  SourceParserStub exprParser;
  auto pkb = make_unique<QPSQueryPKBStub>(&pkbStore);
  auto qps = make_unique<QueryDriver>(pkb.get(), &exprParser);

  qps->evaluate("stmt s1, s2; Select s1 such that Follows(s1, s2)");
}
