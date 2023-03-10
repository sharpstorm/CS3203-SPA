#pragma once

#include <memory>

#include "IASTNode.h"

using std::unique_ptr;

class IAST {
 public:
  virtual ~IAST() = default;
  virtual IASTNode* getRoot() = 0;
};

typedef unique_ptr<IAST> IASTPtr;
