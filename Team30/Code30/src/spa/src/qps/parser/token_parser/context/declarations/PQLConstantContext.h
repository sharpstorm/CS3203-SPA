#pragma once

#include "PQLDeclarationContext.h"

class PQLConstantContext: public PQLDeclarationContext {
 public:
  PQLConstantContext(): PQLDeclarationContext(PQL_VAR_TYPE_CONSTANT) {}
};
