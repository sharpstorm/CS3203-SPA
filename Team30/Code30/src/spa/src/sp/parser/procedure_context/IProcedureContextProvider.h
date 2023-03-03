#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "ProcedureContextType.h"
#include "sp/parser/IContextProvider.h"

#include "../entity_context/IEntityParser.h"
#include "../conditional_context/ConditionalParser.h"
#include "../expression_context/ExpressionParser.h"

class IProcedureContextProvider: public IContextProvider<ProcedureContextType> {
 public:
  virtual IEntityParser* getEntityParser() = 0;
  virtual IConditionalParser* getConditionalParser() = 0;
  virtual IExpressionParser* getExpressionParser() = 0;
};

typedef RecursiveParseContext<IProcedureContextProvider>
    RecursiveProcedureParseContext;
