#pragma once

#include <string>

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IIfPatternWriter.h"

using std::string;

class IfPatternWriter : public IIfPatternWriter {
 public:
  explicit IfPatternWriter(IfPatternStorage *store);

  void addIfPattern(int stmt, string variable) override;

 private:
  IfPatternStorage *store;
};
