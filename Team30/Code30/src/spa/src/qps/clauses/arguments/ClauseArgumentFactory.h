#pragma once

#include <string>
#include <memory>

#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"

using std::string, std::unique_ptr;

class ClauseArgumentFactory {
 public:
  static ClauseArgumentPtr createWildcard();
  static ClauseArgumentPtr create(PQLQuerySynonymProxy synProxy);
  static ClauseArgumentPtr create(const int &stmt);
  static ClauseArgumentPtr create(const string &ident);
};
