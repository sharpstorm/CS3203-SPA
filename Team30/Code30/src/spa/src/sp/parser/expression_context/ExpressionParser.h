#pragma once

#include "common/ast/ASTNode.h"

#include "../SourceParseState.h"
#include "../SubContextParser.h"
#include "ExpressionContextType.h"

class ExpressionParser: public SubContextParser<
    ExpressionContextType, ExpressionContextType::EXPR_CONTEXT> {
 public:
  virtual ~ExpressionParser() = default;
};

typedef SubContextParser<ExpressionContextType,
                         ExpressionContextType::EXPR_CONTEXT>
    IExpressionParser;
