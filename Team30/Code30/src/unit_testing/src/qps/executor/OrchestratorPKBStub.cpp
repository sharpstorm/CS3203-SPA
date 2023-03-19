#include "../PKBStub.cpp"

class OrchestratorPKBStub : public StubPKB {
 public:
  OrchestratorPKBStub(PKB *in) : StubPKB(in) {
  }

  unordered_set<string> getSymbolsOfType(EntityType) const {
    return unordered_set<string>({"x", "y", "z"});
  }

  unordered_set<int> getStatementsOfType(StmtType) const {
    return unordered_set<int>({1, 2, 3});
  }
};
