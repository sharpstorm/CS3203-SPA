#pragma once

#include "ConditionalContextType.h"
#include "../SubContextParser.h"

class ConditionalParser: public SubContextParser<
    ConditionalContextType, ConditionalContextType::COND_CONTEXT> {
 public:
  virtual ~ConditionalParser() = default;
};

typedef SubContextParser<ConditionalContextType,
                         ConditionalContextType::COND_CONTEXT>
    IConditionalParser;
