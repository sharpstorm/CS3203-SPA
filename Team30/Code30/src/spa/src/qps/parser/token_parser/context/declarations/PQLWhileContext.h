#pragma once

#include "PQLDeclarationContext.h"

class PQLWhileContext: public PQLDeclarationContext {
 public:
  PQLWhileContext(): PQLDeclarationContext(PQL_VAR_TYPE_WHILE) {}
};
