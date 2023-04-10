#include <string>
#include "PKBStub.cpp"

using std::string;

class QPSQueryPKBStub : public StubPKB {
 public:
  QPSQueryPKBStub(PKB *in) : StubPKB(in) {
  }

  QueryResultPtr<StmtValue, StmtValue> makeResult(StmtRef leftRef, StmtRef rightRef) const {
    auto result = make_unique<QueryResult<StmtValue, StmtValue>>();
    bool isLeftKnown = leftRef.isKnown() || leftRef.isWildcard();
    bool isRightKnown = rightRef.isKnown() || rightRef.isWildcard();
    if (isLeftKnown && isRightKnown) {
      result->setNotEmpty();
    } else if (!isLeftKnown && !isRightKnown) {
      result->addPair(1, 2);
    } else if (isLeftKnown) {
      result->addRight(2);
    } else {
      result->addLeft(1);
    }

    return result;
  }

  QueryResultPtr<StmtValue, StmtValue> queryFollows(StmtRef s1, StmtRef s2) const override {
    return makeResult(s1, s2);
  };
  QueryResultPtr<int, int> queryFollowsStar(StmtRef s1,
                                         StmtRef s2) const override {
    return makeResult(s1, s2);
  };
  QueryResultPtr<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    return makeResult(s1, s2);
  };
  QueryResultPtr<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    return makeResult(s1, s2);
  };
  unordered_set<string> getSymbolsOfType(EntityType) const override {
    return unordered_set<string>();
  };
  unordered_set<int> getStatementsOfType(StmtType) const override {
    return unordered_set<int>();
  };
};
