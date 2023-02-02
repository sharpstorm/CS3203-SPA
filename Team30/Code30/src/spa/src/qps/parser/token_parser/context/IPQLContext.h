#pragma once

#include "../TokenParseState.h"

class IPQLContext {
 public:
  virtual ~IPQLContext() = default;
  virtual void parse(TokenParseState* parserState) = 0;
};
