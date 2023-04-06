#include "pkb/queryHandlers/PkbQueryHandler.h"

class WithSelectPKBStub : public PkbQueryHandler {
 public:
  WithSelectPKBStub(PKB *in): PkbQueryHandler(in) { }
};
