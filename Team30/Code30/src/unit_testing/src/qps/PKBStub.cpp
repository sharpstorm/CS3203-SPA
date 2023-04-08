#include <memory>

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::string, std::make_unique;

class StubPKB : public PkbQueryHandler {
 public:
  StubPKB(PKB *in) : PkbQueryHandler(in) {
  }

  QueryResultPtr<int, int> queryFollows(StmtRef s1, StmtRef s2) const override {
    return make_unique<QueryResult<int,int>>();
  };

  QueryResultPtr<int, int> queryFollowsStar(StmtRef s1,
                                         StmtRef s2) const override {
    return make_unique<QueryResult<int,int>>();
  };

  QueryResultPtr<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    return make_unique<QueryResult<int, int>>();
  };

  QueryResultPtr<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    return make_unique<QueryResult<int, int>>();
  };

  QueryResultPtr<int, string> queryUses(StmtRef, EntityRef) const override {
    return make_unique<QueryResult<int, string>>();
  }

  QueryResultPtr<string, string> queryUses(EntityRef, EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  }

  QueryResultPtr<int, string> queryModifies(StmtRef, EntityRef) const override {
    return make_unique<QueryResult<int, string>>();
  }

  QueryResultPtr<string, string> queryModifies(EntityRef,
                                            EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  }

  QueryResultPtr<int, PatternTrie *> queryAssigns(StmtRef) const override {
    return make_unique<QueryResult<int, PatternTrie *>>();
  }

  QueryResultPtr<string, string> queryCalls(EntityRef, EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  QueryResultPtr<string, string> queryCallsStar(EntityRef,
                                             EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  unordered_set<string> getSymbolsOfType(EntityType) const override {
    return unordered_set<string>();
  };

  unordered_set<int> getStatementsOfType(StmtType) const override {
    return unordered_set<int>();
  };
};
