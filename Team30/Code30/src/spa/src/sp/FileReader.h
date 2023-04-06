#pragma once

#include "SPTypes.h"

class FileReader {
 public:
  static FileDataPtr readFromFile(const FilePath &fileName);
};
