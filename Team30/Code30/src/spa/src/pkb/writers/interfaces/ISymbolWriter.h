#pragma once

#include <string>
#include "common/Types.h"

using std::string;

class ISymbolWriter {
 public:
  virtual void addSymbol(string, EntityType) = 0;
};
