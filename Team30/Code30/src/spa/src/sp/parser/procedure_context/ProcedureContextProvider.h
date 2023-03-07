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
  ProcedureContextProvider(IEntityParser* entityParser,
                           IExpressionParser* exprParser,
                           IConditionalParser* condParser);
  ASTNodePtr generateSubtree(ProcedureContextType type,
                             SourceParseState* state) override;

  ASTNodePtr parseConstant(SourceParseState* state) override;
  ASTNodePtr parseVariable(SourceParseState* state) override;
  ASTNodePtr parseCondition(SourceParseState* state) override;
  ASTNodePtr parseExpression(SourceParseState* state) override;

 private:
  SourceParseContext* getContext(ProcedureContextType type);

  ProcedureContext procedureContext;
  StatementListContext stmtListContext;
  StatementContext stmtContext;

  AssignContext assignContext;
  CallContext callContext;
  IfContext ifContext;
  PrintContext printContext;
  ReadContext readContext;
  WhileContext whileContext;

  IEntityParser* entityParser;
  IExpressionParser* exprParser;
  IConditionalParser* condParser;
};
