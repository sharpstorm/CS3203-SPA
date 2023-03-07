#pragma once

#include "sp/parser/RecursiveParseContext.h"
#include "ProcedureContextType.h"
#include "sp/parser/IContextProvider.h"

#include "../entity_context/IEntityParser.h"
#include "../conditional_context/ConditionalParser.h"
#include "../expression_context/ExpressionParser.h"

class IProcedureContextProvider: public IContextProvider<ProcedureContextType> {
 public:
  virtual ASTNodePtr parseConstant(SourceParseState* state) = 0;
  virtual ASTNodePtr parseVariable(SourceParseState* state) = 0;
  virtual ASTNodePtr parseCondition(SourceParseState* state) = 0;
  virtual ASTNodePtr parseExpression(SourceParseState* state) = 0;
};

typedef RecursiveParseContext<IProcedureContextProvider>
    RecursiveProcedureParseContext;
