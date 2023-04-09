/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/interfaces/IProvider.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

using pkb::Predicate;

template <typename Value, typename Type>
class AbstractPredicateFactory {
 public:
  ~AbstractPredicateFactory() {}

  virtual Predicate<Value> handleType(IRef<Value, Type> *ref) const = 0;

  Predicate<Value> getPredicate(IRef<Value, Type> *ref) const {
    if (ref->isKnown()) {
      return [ref](Value const s) { return s == ref->getValue(); };
    } else if (ref->isType(Type()) || ref->isWildcard()) {
      return [](Value const s) { return true; };
    }
    return handleType(ref);
  }
};
