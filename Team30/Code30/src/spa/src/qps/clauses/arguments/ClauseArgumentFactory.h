#pragma once

#include <string>
#include <memory>

#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"
#include "qps/common/PQLQuerySynonymProxy.h"

using std::string, std::unique_ptr;

class ClauseArgumentFactory {
 public:
  static ClauseArgumentPtr createWildcard();
  static ClauseArgumentPtr create(const PQLQuerySynonymProxy &synProxy);
  static ClauseArgumentPtr create(const int &stmt);
  static ClauseArgumentPtr create(const string &ident);
};
