#pragma once

#include "PQLDeclarationContext.h"

class PQLPrintContext: public PQLDeclarationContext {
 public:
  PQLPrintContext(): PQLDeclarationContext(PQL_VAR_TYPE_PRINT) {}
};
