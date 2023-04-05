#pragma once

#include "../common/SourceToken.h"
#include "sp/ast/AST.h"

#include "sp/parser/entity_context/EntityParser.h"
#include "sp/parser/expression_context/ExpressionParser.h"
#include "sp/parser/conditional_context/ConditionalParser.h"
#include "sp/parser/procedure_context/ProcedureParser.h"

class SourceTokenParser {
 public:
  SourceTokenParser();

  ASTPtr parseProgram(SourceTokenStream *tokens) const;
  ASTPtr parseExpression(SourceTokenStream *tokens) const;

 private:
  IEntityParserPtr entityParser;
  IExpressionParserPtr exprParser;
  IConditionalParserPtr condParser;
  IProcedureParserPtr procedureParser;
};
