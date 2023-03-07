#pragma once

#include <vector>

#include "../common/SourceToken.h"
#include "sp/ast/AST.h"
#include "sp/parser/entity_context/EntityParser.h"
#include "sp/parser/expression_context/ExpressionParser.h"
#include "sp/parser/conditional_context/ConditionalParser.h"
#include "sp/parser/procedure_context/ProcedureParser.h"

using std::vector;

class SourceTokenParser {
 public:
  SourceTokenParser();

  AST parseProgram(vector<SourceToken>* tokens);
  AST parseExpression(vector<SourceToken>* tokens);

 private:
  IEntityParserPtr entityParser;
  IExpressionParserPtr exprParser;
  IConditionalParserPtr condParser;
  IProcedureParserPtr procedureParser;
};
