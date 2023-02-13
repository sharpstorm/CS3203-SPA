#pragma once

#include "PQLDeclarationContext.h"

class PQLReadContext: public PQLDeclarationContext {
 public:
  PQLReadContext(): PQLDeclarationContext(PQL_SYN_TYPE_READ) {}
};
