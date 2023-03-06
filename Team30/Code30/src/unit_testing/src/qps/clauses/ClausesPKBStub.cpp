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

  QueryResult<int, string> queryUses(StmtRef sRef, EntityRef eRef) const override {
    if (sRef.isKnown() && eRef.isKnown()) {
     return QueryResult<int, string>();
    }

    auto result = QueryResult<int, string>();
    if (!sRef.isKnown() && eRef.isKnown()) {
      result.add(6, "count");
      result.add(7, "cenX");
      result.add(7, "x");
      result.add(8, "cenY");
      result.add(8, "y");
    } else if (sRef.isKnown() && !eRef.isKnown()) {
      result.add(6, "count");
    } else {
      result.add(6, "count");
      result.add(7, "cenX");
      result.add(7, "x");
      result.add(8, "cenY");
      result.add(8, "y");
    }

    return result;
  };

  QueryResult<string, string> queryUses(EntityRef, EntityRef) const override {
    return QueryResult<string, string>();
  };

  QueryResult<int, string> queryModifies(StmtRef sRef, EntityRef eRef) const override {
    if (sRef.isKnown() && eRef.isKnown()) {
      return QueryResult<int, string>();
    }

    auto result = QueryResult<int, string>();
    if (!sRef.isKnown() && eRef.isKnown()) {
      result.add(1, "count");
      result.add(6, "count");
    } else if (sRef.isKnown() && !eRef.isKnown()) {
      result.add(8, "cenY");
    } else {
      result.add(1, "count");
      result.add(2, "cenX");
      result.add(3, "cenY");
      result.add(6, "count");
      result.add(7, "cenX");
      result.add(8, "cenY");
    }

    return result;
  }

  QueryResult<string, string> queryModifies(EntityRef,
                                            EntityRef) const override {
    return QueryResult<string, string>();
  };

  QueryResult<string, string> queryCalls(EntityRef e1, EntityRef e2) const override {
    QueryResult<string, string> result;

    if (e1.type == EntityType::None && e2.type == EntityType::None) {
      // Both wildcards and Static Results
      return result;
    }

    if (!e1.isKnown() && e2.isKnown()) {
      // (syn, static)
      result.add("Ironhide", "Barricade");
    } else if (e1.isKnown() && !e2.isKnown()) {
      // (static, syn) or (static, wildcard)
      result.add("Bumblebee", "Megatron");
      result.add("Bumblebee", "Ironhide");
    } else {
      // Both syns or (syn, _) or (_, syn)
      result.add("Bumblebee", "Megatron");
      result.add("Bumblebee", "Ironhide");
      result.add("Ironhide", "Barricade");
    }

    return result;
  };

  QueryResult<string, string> queryCallsStar(EntityRef e1, EntityRef e2) const override {
    QueryResult<string, string> result;

    if (e1.type == EntityType::None && e2.type == EntityType::None) {
      // Both wildcards and Static Results
      return result;
    }

    if (!e1.isKnown() && e2.isKnown()) {
      // (syn, static)
      result.add("Ironhide", "Barricade");
    } else if (e1.isKnown() && !e2.isKnown()) {
      // (static, syn) or (static, wildcard)
      result.add("Bumblebee", "Megatron");
      result.add("Bumblebee", "Ironhide");
      result.add("Bumblebee", "Barricade");
    } else {
      // Both syns or (syn, _) or (_, syn)
      result.add("Bumblebee", "Megatron");
      result.add("Bumblebee", "Ironhide");
      result.add("Bumblebee", "Barricade");
      result.add("Ironhide", "Barricade");
    }

    return result;
  }

  unordered_set<string> getSymbolsOfType(EntityType) const override {
    return unordered_set<string>();
  };
  unordered_set<int> getStatementsOfType(StmtType) const override {
    return unordered_set<int>();
  };

  // Utility functions
  static QueryResult<int, int> createFollowsTResult() {
    auto result = QueryResult<int, int>();
    result.add(1, 2);
    result.add(1, 3);
    result.add(1, 4);
    result.add(2, 3);
    result.add(2, 4);
    result.add(3, 4);
    return result;
  }

  static QueryResult<int, int> createParentTResult() {
    auto result = QueryResult<int, int>();
    result.add(6, 7);
    result.add(6, 8);
    result.add(6, 9);
    return result;
  }
};

