#include "catch.hpp"

#include "qps/IQPS.h"
#include "qps/QPSFacade.h"
#include "PKBStub.cpp"

TEST_CASE("QPS Execute Query") {
  PKB pkbStore;
  PkbQueryHandler* pkb = new StubPKB(&pkbStore);
  auto handler = shared_ptr<PkbQueryHandler>(pkb);
  IQPS* qps = new QPSFacade(handler);

  qps->evaluate("stmt s1, s2; Select s1 such that Follows(s1, s2)");
  delete(qps);
}
