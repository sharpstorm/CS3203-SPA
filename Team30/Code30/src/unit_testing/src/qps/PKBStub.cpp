#include <string>
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::string;

class StubPKB: public PkbQueryHandler {
 public:
  StubPKB(PKB* in): PkbQueryHandler(in) {
  }

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const override {
    return QueryResult<int, int>();
  };

  QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) const override {
    return QueryResult<int, int>();
  };

  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    return QueryResult<int, int>();
  };

  QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    return QueryResult<int, int>();
  };

  QueryResult<int, string> queryUses(StmtRef, EntityRef) const override {
    return QueryResult<int, string>();
  }

  QueryResult<string, string> queryUses(EntityRef, EntityRef) const override {
    return QueryResult<string, string>();
  }

  QueryResult<int, string> queryModifies(StmtRef, EntityRef) const override {
    return QueryResult<int, string>();
  }

  QueryResult<string, string> queryModifies(EntityRef,
                                            EntityRef) const override {
    return QueryResult<string, string>();
  }

  unordered_set<string> getSymbolsOfType(EntityType) const override {
    return unordered_set<string>();
  };
  unordered_set<int> getStatementsOfType(StmtType) const override {
    return unordered_set<int>();
  };
};
