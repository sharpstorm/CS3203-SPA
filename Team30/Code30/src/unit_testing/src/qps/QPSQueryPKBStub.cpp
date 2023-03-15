#include <string>
#include "PKBStub.cpp"

using std::string;

class QPSQueryPKBStub: public StubPKB {
 public:
  QPSQueryPKBStub(PKB* in): StubPKB(in) {
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
  unordered_set<string> getValuesOfType(EntityType) const override{
    return unordered_set<string>();
  };
  unordered_set<int> getValuesOfType(StmtType) const override{
    return unordered_set<int>();
  };
};
