#pragma once

#include "common/Types.h"

class IDesignEntitiesQueryHandler {
 public:
  virtual ~IDesignEntitiesQueryHandler() {}
  virtual EntityValueSet getSymbolsOfType(EntityType) const = 0;
  virtual StmtValueSet getStatementsOfType(StmtType) const = 0;
  virtual StmtType getStatementType(StmtValue) const = 0;
  virtual EntityValue getVariableByIndex(EntityIdx) const = 0;
  virtual EntityValue getConstantByIndex(EntityIdx) const = 0;
  virtual EntityIdx getIndexOfVariable(EntityValue) const = 0;
  virtual EntityIdx getIndexOfConstant(EntityValue) const = 0;
  virtual bool isStatementOfType(StmtType, StmtValue) const = 0;
  virtual bool isSymbolOfType(EntityType, EntityValue) const = 0;
  virtual EntityValue getCalledDeclaration(StmtValue) const = 0;
};
