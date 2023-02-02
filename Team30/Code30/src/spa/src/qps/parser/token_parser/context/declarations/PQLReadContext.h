#pragma once

#include "PQLDeclarationContext.h"

class PQLReadContext: public PQLDeclarationContext {
 public:
  PQLReadContext(): PQLDeclarationContext(PQL_VAR_TYPE_READ) {}
};
