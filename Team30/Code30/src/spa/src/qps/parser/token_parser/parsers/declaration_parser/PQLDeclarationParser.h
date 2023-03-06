#pragma once

#include "../IPQLParser.h"
#include "contexts/PQLDeclarationContext.h"

#include "contexts/PQLDeclarationContextTypes.h"

class PQLDeclarationParser: IPQLParser {
 public:
  void parse(QueryTokenParseState* parserState, QueryBuilder *queryBuilder);

 private:
  IPQLDeclarationContext* getContext(PQLTokenType type);

  PQLAssignContext assignContext;
  PQLCallContext callContext;
  PQLConstantContext constantContext;
  PQLIfContext ifContext;
  PQLPrintContext printContext;
  PQLProcedureContext procedureContext;
  PQLReadContext readContext;
  PQLStmtContext stmtContext;
  PQLVariableContext variableContext;
  PQLWhileContext whileContext;
};
