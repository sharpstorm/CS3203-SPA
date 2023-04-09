#pragma once

#include "common/Types.h"

class ISymbolWriter {
 public:
  virtual ~ISymbolWriter() {}
  virtual void addSymbol(EntityValue, EntityType) = 0;
};
