#include <unordered_set>

#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::unordered_set;

class WithSelectPKBStub : public PkbQueryHandler {
 public:
  WithSelectPKBStub(PKB *in): PkbQueryHandler(in) { }

  unordered_set<int> getStatementsOfType(StmtType type) const override {
    return unordered_set<int>({1, 2, 3});
  }

  string getPrintDeclarations(int stmt) const override {
    if (stmt == 2)  {
      return "x";
    }

    return "y";
  }

  string getReadDeclarations(int stmt) const override {
    if (stmt == 1) {
      return "x";
    }

    return "y";
  }

  string getCalledDeclaration(int stmt) const override {
    if (stmt == 3) {
      return "x";
    }

    return "y";
  }


  bool isStatementOfType(StmtType type, int value) const override {
    if (type == StmtType::Read && value == 1) {
      return true;
    }

    if (type == StmtType::Print && value == 2) {
      return true;
    }

    if (type == StmtType::Call && value == 3) {
      return true;
    }

    return false;
  }
};
