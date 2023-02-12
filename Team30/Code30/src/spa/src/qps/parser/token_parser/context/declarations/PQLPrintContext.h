#pragma once

#include "PQLDeclarationContext.h"

class PQLPrintContext: public PQLDeclarationContext {
 public:
  PQLPrintContext(): PQLDeclarationContext(PQL_SYN_TYPE_PRINT) {}
};
