#pragma once

#include "PQLDeclarationContext.h"

class PQLWhileContext: public PQLDeclarationContext {
 public:
  PQLWhileContext(): PQLDeclarationContext(PQL_SYN_TYPE_WHILE) {}
};
