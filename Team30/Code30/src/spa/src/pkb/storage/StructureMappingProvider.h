#pragma once

#include "../../common/Types.h"

class StructureMappingProvider {
 public:
  bool isStatementOfType(int s, StmtType stmtType) const;
};
