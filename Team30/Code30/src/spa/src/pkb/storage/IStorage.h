#pragma once

#include <unordered_set>

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"

using pkb::Predicate, std::unordered_set;

template <typename K, typename V>
class IStorage {
 public:
  virtual ~IStorage() = default;

  virtual void insert(K leftArg, V rightArg) = 0;

  /**
   * Find R(leftArg, rightArg) where leftArg and rightArg are given values.
   */
  virtual QueryResultPtr<K, V> query(K leftArg, V rightArg,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Find R(leftArg, rightArg) where leftArg is in the given arg1Values and
   * rightArg satisfies arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(const unordered_set<K> &arg1Values,
                                     Predicate<V> arg2Predicate,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Find R(leftArg, rightArg) where rightArg is in the given arg2Values and
   * leftArg satisfies arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate,
                                     const unordered_set<V> &arg2Values,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Return non-empty result if at least one such relation
   */
  virtual QueryResultPtr<K, V> hasRelation(
      QueryResultBuilder<K, V> *) const = 0;

  /**
   * Right side wildcard.
   */
  virtual QueryResultPtr<K, V> rightWildcardQuery(
      const unordered_set<K> &arg1Values, QueryResultBuilder<K, V> *) const = 0;

  /**
   * Left side wildcard.
   */
  virtual QueryResultPtr<K, V> leftWildcardQuery(
      const unordered_set<V> &arg2Values, QueryResultBuilder<K, V> *) const = 0;
};
