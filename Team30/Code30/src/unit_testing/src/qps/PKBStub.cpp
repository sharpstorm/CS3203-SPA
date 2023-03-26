#include <memory>
#include <string>

#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::string, std::make_unique;

class StubPKB : public PkbQueryHandler {
 public:
  StubPKB(PKB *in) : PkbQueryHandler(in) {
  }

  unique_ptr<QueryResult<int, int>> queryFollows(StmtRef s1, StmtRef s2) const override {
    return make_unique<QueryResult<int,int>>();
  };

  unique_ptr<QueryResult<int, int>> queryFollowsStar(StmtRef s1,
                                         StmtRef s2) const override {
    return make_unique<QueryResult<int,int>>();
  };

  unique_ptr<QueryResult<int, int>> queryParent(StmtRef s1, StmtRef s2) const override {
    return make_unique<QueryResult<int, int>>();
  };

  unique_ptr<QueryResult<int, int>> queryParentStar(StmtRef s1, StmtRef s2) const override {
    return make_unique<QueryResult<int, int>>();
  };

  unique_ptr<QueryResult<int, string>> queryUses(StmtRef, EntityRef) const override {
    return make_unique<QueryResult<int, string>>();
  }

  unique_ptr<QueryResult<string, string>> queryUses(EntityRef, EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  }

  unique_ptr<QueryResult<int, string>> queryModifies(StmtRef, EntityRef) const override {
    return make_unique<QueryResult<int, string>>();
  }

  unique_ptr<QueryResult<string, string>> queryModifies(EntityRef,
                                            EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  }

  unique_ptr<QueryResult<int, PatternTrie *>> queryAssigns(StmtRef) const override {
    return make_unique<QueryResult<int, PatternTrie *>>();
  }

  unique_ptr<QueryResult<string, string>> queryCalls(EntityRef, EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  unique_ptr<QueryResult<string, string>> queryCallsStar(EntityRef,
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
