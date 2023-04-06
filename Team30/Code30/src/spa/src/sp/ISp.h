#pragma once
#include "pkb/writers/PkbWriter.h"
#include "SPTypes.h"

class ISp {
 public:
  virtual ~ISp() = default;
  virtual void parseSource(const FilePath &input, PkbWriter *pkbWriter) = 0;
};
