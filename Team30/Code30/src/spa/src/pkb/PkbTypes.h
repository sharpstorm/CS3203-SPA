#pragma once

#include <functional>

#include "common/Types.h"

using std::function;

namespace pkb {
// predicate
template <typename T>
using Predicate = function<bool(T const &)>;

// validators
template <typename Value, typename Type>
using ArgValidator = bool (*)(const IRef<Value, Type> *ref);

typedef ArgValidator<StmtValue, StmtType> StmtRefValidator;
typedef ArgValidator<EntityValue, EntityType> EntityRefValidator;

template <typename Value, typename Type>
constexpr ArgValidator<Value, Type> defaultValidator =
    [](const IRef<Value, Type> *arg) { return true; };

// transformers
template <typename Value, typename Type>
using ArgTransformer = IRef<Value, Type> *(*)(IRef<Value, Type> *ref);

template <typename Value, typename Type>
constexpr ArgTransformer<Value, Type> defaultTransformer =
    [](IRef<Value, Type> *arg) { return arg; };

// function
template <typename K, typename V>
using Function = function<void(const K &key, const V &value)>;
// using Function = void (*)(const K &key, const V &value);

template <typename K, typename V>
using Terminator = function<bool(const K &key, const V &value)>;

}  // namespace pkb
