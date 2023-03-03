#pragma once

#include <memory>

#include "common/ast/ASTNode.h"

#include "../SourceParseState.h"
#include "../SubContextParser.h"
#include "ExpressionContextType.h"

using std::unique_ptr, std::make_unique;

typedef SubContextParser<ExpressionContextType,
                         ExpressionContextType::EXPR_CONTEXT>
    IExpressionParser;

class ExpressionParser: public IExpressionParser {
 public:
  explicit ExpressionParser(IEntityParser* entityParser):
      SubContextParser(make_unique<ExpressionContextProvider>(entityParser)) {}
};

typedef unique_ptr<IExpressionParser> IExpressionParserPtr;
