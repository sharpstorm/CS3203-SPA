#pragma once

#include "PQLDeclarationContext.h"

class PQLCallContext: public PQLDeclarationContext {
 public:
  PQLCallContext(): PQLDeclarationContext(PQL_VAR_TYPE_CALL) {}
};
