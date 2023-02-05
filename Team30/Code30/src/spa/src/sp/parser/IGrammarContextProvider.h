#pragma once

#include "SourceParseState.h"
#include "parse_context/SourceParseContext.h"

enum SourceGrammarContextType {
  PROCEDURE_CONTEXT,
  STMT_LIST_CONTEXT,
  STMT_CONTEXT,

  READ_CONTEXT,
  ASSIGN_CONTEXT,
  PRINT_CONTEXT,
  WHILE_CONTEXT,
  IF_CONTEXT,

  COND_CONTEXT,
  REL_CONTEXT,
  REL_FACTOR_CONTEXT,

  EXPR_CONTEXT,
  TERM_CONTEXT,
  FACTOR_CONTEXT,

  VARIABLE_CONTEXT,
  CONST_CONTEXT,
};

class IGrammarContextProvider {
 public:
  virtual SourceParseContext* getContext(SourceGrammarContextType type) = 0;
  virtual int currLineCounter() = 0;
  virtual void advanceLineCounter() = 0;
};
