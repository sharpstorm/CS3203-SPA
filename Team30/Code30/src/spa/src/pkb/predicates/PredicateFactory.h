#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/interfaces/IProvider.h"

using std::string;

class PredicateFactory {
 public:
  PredicateFactory();

  template <typename V, typename T>
  Predicate<V> getPredicate(IRef<T, V>*, IProvider<V, T>*) const;
};
