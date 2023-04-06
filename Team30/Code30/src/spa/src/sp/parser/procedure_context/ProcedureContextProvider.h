#pragma once

#include "../SourceParseContext.h"
#include "ProcedureContextType.h"
#include "IProcedureContextProvider.h"

#include "contexts/ProcedureContext.h"
#include "contexts/StatementListContext.h"
#include "contexts/StatementContext.h"
#include "contexts/statement_types/AssignContext.h"
#include "contexts/statement_types/CallContext.h"
#include "contexts/statement_types/IfContext.h"
#include "contexts/statement_types/PrintContext.h"
#include "contexts/statement_types/ReadContext.h"
#include "contexts/statement_types/WhileContext.h"

class ProcedureContextProvider : public IProcedureContextProvider {
 public:
  ProcedureContextProvider(IEntityParser *entityParser,
                           IExpressionParser *exprParser,
                           IConditionalParser *condParser);
  ASTNodePtr generateSubtree(ProcedureContextType type,
                             SourceParseState *state) const override;

  ASTNodePtr parseVariable(SourceParseState *state) const override;
  ASTNodePtr parseCondition(SourceParseState *state) const override;
  ASTNodePtr parseExpression(SourceParseState *state) const override;

 private:
  const SourceParseContext *getContext(ProcedureContextType type) const;

  ProcedureContext procedureContext;
  StatementListContext stmtListContext;
  StatementContext stmtContext;

  AssignContext assignContext;
  CallContext callContext;
  IfContext ifContext;
  PrintContext printContext;
  ReadContext readContext;
  WhileContext whileContext;

  IEntityParser *entityParser;
  IExpressionParser *exprParser;
  IConditionalParser *condParser;
};
