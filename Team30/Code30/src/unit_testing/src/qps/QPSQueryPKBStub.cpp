#include <string>
#include "PKBStub.cpp"

using std::string;

class QPSQueryPKBStub : public StubPKB {
 public:
  QPSQueryPKBStub(PKB *in) : StubPKB(in) {
  }

  QueryResultPtr<int, int> queryFollows(StmtRef s1, StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    result->add(1, 2);
    return result;
  };
  QueryResultPtr<int, int> queryFollowsStar(StmtRef s1,
                                         StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    result->add(1, 2);
    return result;
  };
  QueryResultPtr<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    result->add(1, 2);
    return result;
  };
  QueryResultPtr<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    result->add(1, 2);
    return result;
  };
  unordered_set<string> getSymbolsOfType(EntityType) const override {
    return unordered_set<string>();
  };
  unordered_set<int> getStatementsOfType(StmtType) const override {
    return unordered_set<int>();
  };
};
