#pragma once

#include <functional>

#include "common/Types.h"

namespace pkb {
  // predicate
  template<typename T>
  using Predicate = std::function<bool(T const &)>;

  // validators
  template<typename Value, typename Type>
  using ArgValidator = bool (*)(const IRef<Value, Type> *ref);

  typedef ArgValidator<StmtValue, StmtType> StmtRefValidator;
  typedef ArgValidator<EntityValue, EntityType> EntityRefValidator;

  template<typename Value, typename Type>
  constexpr ArgValidator<Value, Type> defaultValidator = [](
      const IRef<Value, Type> *arg) {
    return true;
  };

  // transformers
  template<typename Value, typename Type>
  using ArgTransformer = IRef<Value, Type> *(*)(IRef<Value, Type> *ref);

  template<typename Value, typename Type>
  constexpr ArgTransformer<Value, Type> defaultTransformer = [](
      IRef<Value, Type> *arg) {
    return arg;
  };

}  // namespace pkb
