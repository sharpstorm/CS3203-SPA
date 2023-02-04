#include "PQLGrammarContextProvider.h"

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

unique_ptr<IPQLContext> PQLGrammarContextProvider::getContext(PQLToken *token) {
  switch (token->type) {
    case PQL_TOKEN_STMT:
      return std::make_unique<PQLStmtContext>();
    case PQL_TOKEN_READ:
      return std::make_unique<PQLReadContext>();
    case PQL_TOKEN_PRINT:
      return std::make_unique<PQLPrintContext>();
    case PQL_TOKEN_CALL:
      return std::make_unique<PQLCallContext>();
    case PQL_TOKEN_WHILE:
      return std::make_unique<PQLWhileContext>();
    case PQL_TOKEN_IF:
      return std::make_unique<PQLIfContext>();
    case PQL_TOKEN_ASSIGN:
      return std::make_unique<PQLAssignContext>();
    case PQL_TOKEN_VARIABLE:
      return std::make_unique<PQLVariableContext>();
    case PQL_TOKEN_CONSTANT:
      return std::make_unique<PQLConstantContext>();
    case PQL_TOKEN_PROCEDURE:
      return std::make_unique<PQLProcedureContext>();
    case PQL_TOKEN_SELECT:
      return std::make_unique<PQLSelectContext>();
    case PQL_TOKEN_SUCH:
      return std::make_unique<PQLSuchThatContext>();
    case PQL_TOKEN_FOLLOWS:
      return std::make_unique<PQLFollowsClauseContext>();
    case PQL_TOKEN_PARENT:
      return std::make_unique<PQLParentClauseContext>();
    case PQL_TOKEN_USES:
      return std::make_unique<PQLUsesClauseContext>();
    case PQL_TOKEN_MODIFIES:
      return std::make_unique<PQLModifiesClauseContext>();
    default:
      return nullptr;
  }
}
