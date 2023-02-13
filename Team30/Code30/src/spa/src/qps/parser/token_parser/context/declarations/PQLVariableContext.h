#pragma once

#include "PQLDeclarationContext.h"

class PQLVariableContext: public PQLDeclarationContext {
 public:
  PQLVariableContext(): PQLDeclarationContext(PQL_SYN_TYPE_VARIABLE) {}
};
