#pragma once

#include "PQLDeclarationContext.h"

class PQLCallContext: public PQLDeclarationContext {
 public:
  PQLCallContext(): PQLDeclarationContext(PQL_SYN_TYPE_CALL) {}
};
