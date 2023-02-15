#pragma once

#include "PQLDeclarationContext.h"

class PQLProcedureContext: public PQLDeclarationContext {
 public:
  PQLProcedureContext(): PQLDeclarationContext(PQL_SYN_TYPE_PROCEDURE) {}
};
