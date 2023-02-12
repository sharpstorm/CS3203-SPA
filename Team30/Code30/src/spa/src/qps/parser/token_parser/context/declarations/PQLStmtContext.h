#pragma once

#include "PQLDeclarationContext.h"

class PQLStmtContext: public PQLDeclarationContext {
 public:
  PQLStmtContext(): PQLDeclarationContext(PQL_SYN_TYPE_STMT) {}
};
