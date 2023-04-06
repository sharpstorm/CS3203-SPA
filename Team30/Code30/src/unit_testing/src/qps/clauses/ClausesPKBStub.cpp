#include <memory>
#include <string>

#include "../PKBStub.cpp"

using std::string, std::unique_ptr, std::make_unique;

class ClausesPKBStub : public StubPKB {
 public:
  ClausesPKBStub(PKB *in) : StubPKB(in) {
  }

  QueryResultPtr<int, int> queryFollows(StmtRef s1, StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    // Static result but s2 > s1 (Does not hold)
    if (s1.isKnown() && s2.isKnown() && s2.getValue() <= s1.getValue()) {
      return result;
    }

    result->add(1, 2);
    return result;
  };

  QueryResultPtr<int, int> queryFollowsStar(StmtRef s1,
                                         StmtRef s2) const override {
    if (s1.isKnown() && s2.isKnown() && s2.getValue() > s1.getValue()) {
      return make_unique<QueryResult<int, int>>();
    }

    return createFollowsTResult();
  };

  QueryResultPtr<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    if (s1.isKnown() && s2.isKnown() && s2.getValue() > s1.getValue()) {
      return result;
    }

    result->add(6, 7);
    return result;
  };

  QueryResultPtr<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override {
    if (s1.isKnown() && s2.isKnown() && s2.getValue() > s1.getValue()) {
      return make_unique<QueryResult<int, int>>();
    }

    return createParentTResult();
  };

  QueryResultPtr<int, string> queryUses(StmtRef sRef,
                                     EntityRef eRef) const override {
    auto result = make_unique<QueryResult<int, string>>();
    if (sRef.isKnown() && eRef.isKnown()) {
      return result;
    }

    if (!sRef.isKnown() && eRef.isKnown()) {
      result->add(6, "count");
      result->add(7, "cenX");
      result->add(7, "x");
      result->add(8, "cenY");
      result->add(8, "y");
    } else if (sRef.isKnown() && !eRef.isKnown()) {
      result->add(6, "count");
    } else {
      result->add(6, "count");
      result->add(7, "cenX");
      result->add(7, "x");
      result->add(8, "cenY");
      result->add(8, "y");
    }

    return result;
  };

  QueryResultPtr<string, string> queryUses(EntityRef, EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  QueryResultPtr<int, string> queryModifies(StmtRef sRef,
                                         EntityRef eRef) const override {
    auto result = make_unique<QueryResult<int, string>>();
    if (sRef.isKnown() && eRef.isKnown()) {
      return result;
    }

    if (!sRef.isKnown() && eRef.isKnown()) {
      result->add(1, "count");
      result->add(6, "count");
    } else if (sRef.isKnown() && !eRef.isKnown()) {
      result->add(8, "cenY");
    } else {
      result->add(1, "count");
      result->add(2, "cenX");
      result->add(3, "cenY");
      result->add(6, "count");
      result->add(7, "cenX");
      result->add(8, "cenY");
    }

    return result;
  }

  QueryResultPtr<string, string> queryModifies(EntityRef,
                                            EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  QueryResultPtr<string, string> queryCalls(EntityRef e1,
                                         EntityRef e2) const override {
    auto result = make_unique<QueryResult<string, string>>();

    if (e1.isType(EntityType::None) && e2.isType(EntityType::None)) {
      // Both wildcards and Static Results
      return result;
    }

    if (!e1.isKnown() && e2.isKnown()) {
      // (syn, static)
      result->add("Ironhide", "Barricade");
    } else if (e1.isKnown() && !e2.isKnown()) {
      // (static, syn) or (static, wildcard)
      result->add("Bumblebee", "Megatron");
      result->add("Bumblebee", "Ironhide");
    } else {
      // Both syns or (syn, _) or (_, syn)
      result->add("Bumblebee", "Megatron");
      result->add("Bumblebee", "Ironhide");
      result->add("Ironhide", "Barricade");
    }

    return result;
  };

  QueryResultPtr<string, string> queryCallsStar(EntityRef e1,
                                             EntityRef e2) const override {
    auto result = make_unique<QueryResult<string,string>>();

    if (e1.isType(EntityType::None) && e2.isType(EntityType::None)) {
      // Both wildcards and Static Results
      return result;
    }

    if (!e1.isKnown() && e2.isKnown()) {
      // (syn, static)
      result->add("Ironhide", "Barricade");
    } else if (e1.isKnown() && !e2.isKnown()) {
      // (static, syn) or (static, wildcard)
      result->add("Bumblebee", "Megatron");
      result->add("Bumblebee", "Ironhide");
      result->add("Bumblebee", "Barricade");
    } else {
      // Both syns or (syn, _) or (_, syn)
      result->add("Bumblebee", "Megatron");
      result->add("Bumblebee", "Ironhide");
      result->add("Bumblebee", "Barricade");
      result->add("Ironhide", "Barricade");
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
  static QueryResultPtr<int, int> createFollowsTResult() {
    auto result = make_unique<QueryResult<int, int>>();
    result->add(1, 2);
    result->add(1, 3);
    result->add(1, 4);
    result->add(2, 3);
    result->add(2, 4);
    result->add(3, 4);
    return result;
  }

  static QueryResultPtr<int, int> createParentTResult() {
    auto result = make_unique<QueryResult<int, int>>();
    result->add(6, 7);
    result->add(6, 8);
    result->add(6, 9);
    return result;
  }
};

