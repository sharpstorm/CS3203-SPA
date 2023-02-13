#pragma once

#include "PQLDeclarationContext.h"

class PQLAssignContext: public PQLDeclarationContext {
 public:
  PQLAssignContext(): PQLDeclarationContext(PQL_SYN_TYPE_ASSIGN) {}
};
