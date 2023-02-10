#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include <string>

using std::string;

class StubPKB: public PkbQueryHandler {
 public:
  StubPKB(PKB* in): PkbQueryHandler(in) {
  }

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    return result;
  };
  QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    return result;
  };
  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    return result;
  };
  QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    return result;
  };
  unordered_set<string> getSymbolsOfType(EntityType) const {
    return unordered_set<string>();
  };
  unordered_set<int> getStatementsOfType(StmtType) const {
    return unordered_set<int>();
  };
};
