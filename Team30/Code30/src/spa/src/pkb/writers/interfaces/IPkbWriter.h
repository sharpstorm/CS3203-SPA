#pragma once

#include <string>
#include "common/Types.h"
#include "pkb/writers/interfaces/IFollowsWriter.h"
#include "pkb/writers/interfaces/IParentWriter.h"
#include "pkb/writers/interfaces/IStatementWriter.h"
#include "pkb/writers/interfaces/ISymbolWriter.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"

using std::string;

class IPkbWriter
    : public IFollowsWriter,
      public IParentWriter,
      public IStatementWriter,
      public ISymbolWriter,
      public IProcedureWriter {
 public:
  virtual ~IPkbWriter() {};
};
