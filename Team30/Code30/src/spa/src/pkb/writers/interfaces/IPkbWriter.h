#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/writers/interfaces/IAssignsWriter.h"
#include "pkb/writers/interfaces/ICallsWriter.h"
#include "pkb/writers/interfaces/IFollowsWriter.h"
#include "pkb/writers/interfaces/IModifiesWriter.h"
#include "pkb/writers/interfaces/IParentWriter.h"
#include "pkb/writers/interfaces/IProcedureWriter.h"
#include "pkb/writers/interfaces/IStatementWriter.h"
#include "pkb/writers/interfaces/ISymbolWriter.h"
#include "pkb/writers/interfaces/IUsesWriter.h"
#include "pkb/writers/interfaces/IPostProcessWriter.h"
#include "IIfPatternWriter.h"

using std::string;

class IPkbWriter : public IFollowsWriter,
                   public IParentWriter,
                   public IStatementWriter,
                   public ISymbolWriter,
                   public IProcedureWriter,
                   public IUsesWriter,
                   public IModifiesWriter,
                   public IAssignsWriter,
                   public ICallsWriter,
                   public IIfPatternWriter,
                   public IPostProcessWriter {
 public:
  virtual ~IPkbWriter() {}
};
