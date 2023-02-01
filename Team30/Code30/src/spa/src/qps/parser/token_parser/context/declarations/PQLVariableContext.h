#pragma once

#include "PQLDeclarationContext.h"

class PQLVariableContext: public PQLDeclarationContext {
 public:
  PQLVariableContext(): PQLDeclarationContext(PQL_VAR_TYPE_VARIABLE) {}
};
