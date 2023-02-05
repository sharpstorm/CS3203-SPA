#pragma once

#include "../QueryTokenParseState.h"

class IPQLContext {
 public:
  virtual ~IPQLContext() = default;
  virtual void parse(QueryTokenParseState* parserState) = 0;
};
