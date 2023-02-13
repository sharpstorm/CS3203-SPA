#pragma once

#include "PQLDeclarationContext.h"

class PQLIfContext: public PQLDeclarationContext {
 public:
  PQLIfContext(): PQLDeclarationContext(PQL_SYN_TYPE_IF) {}
};
