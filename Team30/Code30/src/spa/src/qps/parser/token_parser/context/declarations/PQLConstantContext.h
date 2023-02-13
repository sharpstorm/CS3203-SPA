#pragma once

#include "PQLDeclarationContext.h"

class PQLConstantContext: public PQLDeclarationContext {
 public:
  PQLConstantContext(): PQLDeclarationContext(PQL_SYN_TYPE_CONSTANT) {}
};
