#pragma once

#include "PQLDeclarationContext.h"

class PQLAssignContext: public PQLDeclarationContext {
 public:
  PQLAssignContext(): PQLDeclarationContext(PQL_VAR_TYPE_ASSIGN) {}
};
