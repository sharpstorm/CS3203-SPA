#pragma once

#include "PQLDeclarationContext.h"

class PQLProcedureContext: public PQLDeclarationContext {
 public:
  PQLProcedureContext(): PQLDeclarationContext(PQL_VAR_TYPE_PROCEDURE) {}
};
