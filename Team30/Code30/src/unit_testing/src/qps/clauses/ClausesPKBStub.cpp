#include <string>

#include "../PKBStub.cpp"

using std::string;


class ClausesPKBStub : public StubPKB {
 public:
  ClausesPKBStub(PKB* in): StubPKB(in) {
  }

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    // E.g. Follows(stmtNum, stmtNum)
    if (s1.isKnown() && s2.isKnown()) {
      // Failing this case implies something similar to Follows(5,3)
      if (s1.lineNum <= s2.lineNum) {
        result.add(1, 2);
      }
    } else if (s1.isKnown() || s2.isKnown()) {
      // Follows(1, a) || Follows(a,2)
      result.add(1, 2);
    } else {
      //  Follows(a1,a2)
      result.add(1,2);
    }

    return result;
  };

  QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) const override {
//    auto result = QueryResult<int, int>();
    // E.g. Follows*(stmtNum, stmtNum)
    if (s1.isKnown() && s2.isKnown()) {
      // Failing this case implies something similar to Follows*(5,3)
      if (s1.lineNum <= s2.lineNum) {
        return createFollowsTResult();
      }
    } else if (s1.isKnown() || s2.isKnown()) {
      // Follows*(1, a) || Follows*(a,4)
      return createFollowsTResult();
    } else {
      //  Follows*(a1,a2)
      return createFollowsTResult();
    }

    return QueryResult<int, int>();
  };

  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    // E.g. Parent(stmtNum, stmtNum)
    if (s1.isKnown() && s2.isKnown()) {
      // Failing this case implies something similar to Parent(5,3)
      if (s1.lineNum <= s2.lineNum) {
        result.add(6, 7);
      }
    } else if (s1.isKnown() || s2.isKnown()) {
      // Parent(1, a) || Parent(a,2)
      result.add(6, 7);
    } else {
      //  Parent(a1,a2)
      result.add(6,7);
    }

    return result;
  };

  QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    auto result = QueryResult<int, int>();
    // E.g. Parent*(stmtNum, stmtNum)
    if (s1.isKnown() && s2.isKnown()) {
      // Failing this case implies something similar to Parent*(5,3)
      if (s1.lineNum <= s2.lineNum) {
        return createParentTResult();
      }
    } else if (s1.isKnown() || s2.isKnown()) {
      // Parent*(1, a) || Parent*(a,2)
      return createParentTResult();
    } else {
      //  Parent*(a1, a2)
      return createParentTResult();
    }

    return result;
  };

  unordered_set<string> getSymbolsOfType(EntityType) const {
    return unordered_set<string>();
  };
  unordered_set<int> getStatementsOfType(StmtType) const {
    return unordered_set<int>();
  };

  // Utility functions
  QueryResult<int, int> createFollowsTResult() const {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    result.add(1, 3);
    result.add(1, 4);
    result.add(2, 3);
    result.add(2, 4);
    result.add(3, 4);
    return result;
  }

  QueryResult<int, int> createParentTResult() const {
    auto result = QueryResult<int, int>();
    result.add(6, 7);
    result.add(6, 8);
    result.add(6, 9);
    return result;
  }
};

