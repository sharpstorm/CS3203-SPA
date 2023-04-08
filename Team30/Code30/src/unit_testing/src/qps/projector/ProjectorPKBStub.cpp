#include "pkb/queryHandlers/PkbQueryHandler.h"

class ProjectorPKBStub : public PkbQueryHandler {
 public:
  ProjectorPKBStub(PKB *in) : PkbQueryHandler(in) { }

  string getCalledDeclaration(int value) const override {
    return "callProc";
  }

  string getPrintDeclarations(int value) const override {
    return "printVar";
  }

  string getReadDeclarations(int value) const override {
    return "readVar";
  }
};