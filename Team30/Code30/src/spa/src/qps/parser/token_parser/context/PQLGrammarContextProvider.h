#pragma once

#include "IPQLContext.h"

#include "declarations/PQLStmtContext.h"
#include "declarations/PQLReadContext.h"
#include "declarations/PQLPrintContext.h"
#include "declarations/PQLCallContext.h"
#include "declarations/PQLWhileContext.h"
#include "declarations/PQLIfContext.h"
#include "declarations/PQLAssignContext.h"
#include "declarations/PQLVariableContext.h"
#include "declarations/PQLConstantContext.h"
#include "declarations/PQLProcedureContext.h"

#include "query/PQLSelectContext.h"
#include "query/PQLSuchThatContext.h"

#include "such_that_clause/PQLFollowsClauseContext.h"
#include "such_that_clause/PQLParentClauseContext.h"
#include "such_that_clause/PQLUsesClauseContext.h"
#include "such_that_clause/PQLModifiesClauseContext.h"

#include "pattern_clause/PQLAssignPatternClauseContext.h"

class PQLGrammarContextProvider {
 private:
  PQLStmtContext stmtContext;
  PQLReadContext readContext;
  PQLPrintContext printContext;
  PQLCallContext callContext;
  PQLWhileContext whileContext;
  PQLIfContext ifContext;
  PQLAssignContext assignContext;
  PQLVariableContext variableContext;
  PQLVariableContext constantContext;
  PQLProcedureContext procedureContext;
  PQLSelectContext selectContext;
  PQLSuchThatContext suchThatContext;
  PQLFollowsClauseContext followsClauseContext;
  PQLParentClauseContext parentClauseContext;
  PQLUsesClauseContext usesClauseContext;
  PQLModifiesClauseContext modifiesClauseContext;
  PQLAssignPatternClauseContext assignPatternClauseContext;

 public:
  IPQLContext* getContext(PQLToken* token);
};
