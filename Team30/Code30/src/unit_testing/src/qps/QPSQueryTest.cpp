#include "catch.hpp"

#include "qps/IQPS.h"
#include "qps/QPSFacade.h"
#include "QPSQueryPKBStub.cpp"
#include "sp/SpFacade.h"

TEST_CASE("QPS Execute Query") {
  PKB pkbStore;
  SpFacade exprParser;
  auto pkb = make_unique<QPSQueryPKBStub>(&pkbStore);
  auto qps = make_unique<QPSFacade>(pkb.get(), &exprParser);

  qps->evaluate("stmt s1, s2; Select s1 such that Follows(s1, s2)");
}
