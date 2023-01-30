#pragma once

#include <string>
#include "../common/PQLQuery.h"

using std::string;

class TokenParser {
 public:
  void addTokens(string* tokens);
  PQLQuery* build();
};
