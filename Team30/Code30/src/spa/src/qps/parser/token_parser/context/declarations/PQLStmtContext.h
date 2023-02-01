#pragma once

#include "PQLDeclarationContext.h"

class PQLStmtContext: public PQLDeclarationContext {
 public:
  PQLStmtContext(): PQLDeclarationContext(PQL_VAR_TYPE_STMT) {}
};
