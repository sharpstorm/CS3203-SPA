#pragma once

#include "PQLDeclarationContext.h"

class PQLIfContext: public PQLDeclarationContext {
 public:
  PQLIfContext(): PQLDeclarationContext(PQL_VAR_TYPE_IF) {}
};
