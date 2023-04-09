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

    addTo(result.get(), 1, 2, s1, s2);
    return result;
  };

  QueryResultPtr<int, int> queryFollowsStar(StmtRef s1,
                                            StmtRef s2) const override {
    if (s1.isKnown() && s2.isKnown() && s2.getValue() > s1.getValue()) {
      return make_unique<QueryResult<int, int>>();
    }

    return createFollowsTResult(s1, s2);
  };

  QueryResultPtr<int, int> queryParent(StmtRef s1, StmtRef s2) const override {
    auto result = make_unique<QueryResult<int, int>>();
    if (s1.isKnown() && s2.isKnown() && s2.getValue() > s1.getValue()) {
      return result;
    }

    addTo(result.get(), 6, 7, s1, s2);
    return result;
  };

  QueryResultPtr<int, int> queryParentStar(StmtRef s1,
                                           StmtRef s2) const override {
    if (s1.isKnown() && s2.isKnown() && s2.getValue() > s1.getValue()) {
      return make_unique<QueryResult<int, int>>();
    }

    return createParentTResult(s1, s2);
  };

  QueryResultPtr<int, string> queryUses(StmtRef sRef,
                                        EntityRef eRef) const override {
    auto result = make_unique<QueryResult<int, string>>();
    if (sRef.isKnown() && eRef.isKnown()) {
      return result;
    }

    if (!sRef.isKnown() && eRef.isKnown()) {
      addTo<int, string>(result.get(), 6, "count", sRef, eRef);
      addTo<int, string>(result.get(), 7, "cenX", sRef, eRef);
      addTo<int, string>(result.get(), 7, "x", sRef, eRef);
      addTo<int, string>(result.get(), 8, "cenY", sRef, eRef);
      addTo<int, string>(result.get(), 8, "y", sRef, eRef);
    } else if (sRef.isKnown() && !eRef.isKnown()) {
      addTo<int, string>(result.get(), 6, "count", sRef, eRef);
    } else {
      addTo<int, string>(result.get(), 6, "count", sRef, eRef);
      addTo<int, string>(result.get(), 7, "cenX", sRef, eRef);
      addTo<int, string>(result.get(), 7, "x", sRef, eRef);
      addTo<int, string>(result.get(), 8, "cenY", sRef, eRef);
      addTo<int, string>(result.get(), 8, "y", sRef, eRef);
    }

    return result;
  };

  QueryResultPtr<string, string> queryUses(EntityRef,
                                           EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  QueryResultPtr<int, string> queryModifies(StmtRef sRef,
                                            EntityRef eRef) const override {
    auto result = make_unique<QueryResult<int, string>>();
    if (sRef.isKnown() && eRef.isKnown()) {
      return result;
    }

    if (!sRef.isKnown() && eRef.isKnown()) {
      addTo<int, string>(result.get(), 1, "count", sRef, eRef);
      addTo<int, string>(result.get(), 6, "count", sRef, eRef);
    } else if (sRef.isKnown() && !eRef.isKnown()) {
      addTo<int, string>(result.get(), 8, "cenY", sRef, eRef);
    } else {
      addTo<int, string>(result.get(), 1, "count", sRef, eRef);
      addTo<int, string>(result.get(), 2, "cenX", sRef, eRef);
      addTo<int, string>(result.get(), 3, "cenY", sRef, eRef);
      addTo<int, string>(result.get(), 6, "count", sRef, eRef);
      addTo<int, string>(result.get(), 7, "cenX", sRef, eRef);
      addTo<int, string>(result.get(), 8, "cenY", sRef, eRef);
    }

    return result;
  }

  QueryResultPtr<string, string> queryModifies(EntityRef,
                                               EntityRef) const override {
    return make_unique<QueryResult<string, string>>();
  };

  QueryResultPtr<string, string> queryCalls(EntityRef e1,
                                            EntityRef e2) const override {
    auto result = make_unique<QueryResult<EntityValue, EntityValue>>();

    if (e1.isType(EntityType::None) && e2.isType(EntityType::None)) {
      // Both wildcards and Static Results
      return result;
    }

    if (!e1.isKnown() && e2.isKnown()) {
      // (syn, static)
      addTo<string, string>(result.get(), "Ironhide", "Barricade", e1, e2);
    } else if (e1.isKnown() && !e2.isKnown()) {
      // (static, syn) or (static, wildcard)
      addTo<string, string>(result.get(), "Bumblebee", "Megatron", e1, e2);
      addTo<string, string>(result.get(), "Bumblebee", "Ironhide", e1, e2);
    } else {
      // Both syns or (syn, _) or (_, syn)
      addTo<string, string>(result.get(), "Bumblebee", "Megatron", e1, e2);
      addTo<string, string>(result.get(), "Bumblebee", "Ironhide", e1, e2);
      addTo<string, string>(result.get(), "Ironhide", "Barricade", e1, e2);
    }

    return result;
  };

  QueryResultPtr<string, string> queryCallsStar(EntityRef e1,
                                                EntityRef e2) const override {
    auto result = make_unique<QueryResult<string, string>>();

    if (e1.isType(EntityType::None) && e2.isType(EntityType::None)) {
      // Both wildcards and Static Results
      return result;
    }

    if (!e1.isKnown() && e2.isKnown()) {
      // (syn, static)
      addTo<string, string>(result.get(), "Ironhide", "Barricade", e1, e2);
    } else if (e1.isKnown() && !e2.isKnown()) {
      // (static, syn) or (static, wildcard)
      addTo<string, string>(result.get(), "Bumblebee", "Megatron", e1, e2);
      addTo<string, string>(result.get(), "Bumblebee", "Ironhide", e1, e2);
      addTo<string, string>(result.get(), "Bumblebee", "Barricade", e1, e2);
    } else {
      // Both syns or (syn, _) or (_, syn)
      addTo<string, string>(result.get(), "Bumblebee", "Megatron", e1, e2);
      addTo<string, string>(result.get(), "Bumblebee", "Ironhide", e1, e2);
      addTo<string, string>(result.get(), "Bumblebee", "Barricade", e1, e2);
      addTo<string, string>(result.get(), "Ironhide", "Barricade", e1, e2);
    }

    return result;
  }

  QueryResultPtr<int, string> queryIfPattern(StmtRef stmt,
                                             EntityRef ent) const override {
    auto result = make_unique<QueryResult<int, string>>();
    if (ent.isWildcard()) {
      result->addLeft(1);
      result->addLeft(7);
      return result;
    }

    if (!ent.isKnown()) {
      if (!stmt.isKnown()) {
        result->addPair(1, "x");
        result->addPair(7, "y");
        return result;
      }

      if (stmt.getValue() == 1) {
        result->addRight("x");
      }

      if (stmt.getValue() == 7) {
        result->addRight("y");
      }
      return result;
    }

    if (ent.getValue() == "x") {
      result->addLeft(1);
    }

    return result;
  }

  QueryResultPtr<int, string> queryWhilePattern(StmtRef stmt,
                                                EntityRef ent) const override {
    auto result = make_unique<QueryResult<int, string>>();
    if (ent.isWildcard()) {
      result->addLeft(4);
      result->addLeft(11);
    }

    if (!ent.isKnown()) {
      if (!stmt.isKnown()) {
        result->addPair(4, "x");
        result->addPair(11, "y");
        return result;
      }

      if (stmt.getValue() == 4) {
        result->addRight("x");
      }

      if (stmt.getValue() == 11) {
        result->addRight("y");
      }

      return result;
    }

    if (ent.getValue() == "x") {
      result->addLeft(4);
    }

    if (ent.getValue() == "x") {
      result->addLeft(4);
    }

    return result;
  }

  unordered_set<string> getSymbolsOfType(EntityType) const override {
    return unordered_set<string>();
  };

  unordered_set<int> getStatementsOfType(StmtType) const override {
    return unordered_set<int>();
  };

  StmtType getStatementType(int value) const override {
    if (value == 1) {
      return StmtType::If;
    }

    if (value == 4) {
      return StmtType::While;
    }
    return StmtType::Assign;
  }

  bool isStatementOfType(StmtType type, int value) const override {
    if (type == StmtType::If) {
      return value == 1;
    }

    if (type == StmtType::While) {
      return value == 4;
    }

    return false;
  }

  bool isSymbolOfType(EntityType type, string value) const override {
    if (type == EntityType::Variable) {
      return value == "x" || value == "y";
    }

    return false;
  }

  template<class T, class U, class V, class X>
  static void addTo(QueryResult<T, U> *target,
                    T left, U right,
                    V leftRef, X rightRef) {
    bool isLeftKnown = leftRef.isKnown() || leftRef.isWildcard();
    bool isRightKnown = rightRef.isKnown() || rightRef.isWildcard();
    if (isLeftKnown && isRightKnown) {
      target->setNotEmpty();
    } else if (!isLeftKnown && !isRightKnown) {
      target->addPair(left, right);
    } else if (isLeftKnown) {
      target->addRight(right);
    } else {
      target->addLeft(left);
    }
  }

  // Utility functions
  static QueryResultPtr<int, int> createFollowsTResult(StmtRef leftRef,
                                                       StmtRef rightRef) {
    auto result = make_unique<QueryResult<int, int>>();
    addTo(result.get(), 1, 2, leftRef, rightRef);
    addTo(result.get(), 1, 3, leftRef, rightRef);
    addTo(result.get(), 1, 4, leftRef, rightRef);
    addTo(result.get(), 2, 3, leftRef, rightRef);
    addTo(result.get(), 2, 4, leftRef, rightRef);
    addTo(result.get(), 3, 4, leftRef, rightRef);
    return result;
  }

  static QueryResultPtr<int, int> createParentTResult(StmtRef leftRef,
                                                      StmtRef rightRef) {
    auto result = make_unique<QueryResult<int, int>>();
    addTo(result.get(), 6, 7, leftRef, rightRef);
    addTo(result.get(), 6, 8, leftRef, rightRef);
    addTo(result.get(), 6, 9, leftRef, rightRef);
    return result;
  }
};

