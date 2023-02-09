#include "catch.hpp"

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"

class FollowsPKBStub: public IFollowsQueryHandler {
  virtual QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    return result;
  }

  virtual QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    return result;
  }
};

TEST_CASE("Test QPS Follows Query") {

}
