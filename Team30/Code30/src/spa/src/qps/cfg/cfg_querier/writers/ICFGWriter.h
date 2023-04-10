#pragma once

#include <memory>

#include "common/Types.h"
#include "common/cfg/CFGLinks.h"

using std::unique_ptr;

class ICFGWriter {
 public:
  virtual ~ICFGWriter() = default;
  virtual StmtValue toStmtNumber(CFGNode node) const = 0;
  virtual bool writeBool(StmtValue value) const = 0;
  virtual bool writeLeft(StmtValue value) const = 0;
  virtual bool writeRight(StmtValue value) const = 0;
  virtual void setLeft(StmtValue value) = 0;
  virtual StmtValue getLeft() const = 0;
};

typedef unique_ptr<ICFGWriter> ICFGWriterPtr;
