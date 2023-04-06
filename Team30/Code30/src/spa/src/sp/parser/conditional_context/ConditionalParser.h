#pragma once

#include <memory>

#include "ConditionalContextType.h"
#include "../SubContextParser.h"
#include "../expression_context/ExpressionParser.h"
#include "ConditionalContextProvider.h"

using std::unique_ptr, std::make_unique;

typedef SubContextParser<ConditionalContextType,
                         ConditionalContextType::COND_CONTEXT>
    IConditionalParser;

class ConditionalParser : public IConditionalParser {
 public:
  explicit ConditionalParser(IExpressionParser *exprParser) :
      SubContextParser(make_unique<ConditionalContextProvider>(exprParser)) {}
};

typedef unique_ptr<IConditionalParser> IConditionalParserPtr;
