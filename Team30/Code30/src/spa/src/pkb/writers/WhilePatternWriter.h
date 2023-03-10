#pragma once

#include <string>

#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IWhilePatternWriter.h"

using std::string;

class WhilePatternWriter : public IWhilePatternWriter {
 public:
  explicit WhilePatternWriter(WhilePatternStorage *store);

  void addWhilePattern(int stmt, string variable) override;

 private:
  WhilePatternStorage *store;
};
