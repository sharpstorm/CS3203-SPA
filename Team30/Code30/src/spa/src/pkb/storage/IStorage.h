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

  virtual void insert(K arg1, V arg2) = 0;

  /**
   * Find R(arg1, arg2) where arg1 and arg2 are given values.
   */
  virtual QueryResultPtr<K, V> query(K arg1, V arg2,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(unordered_set<K> arg1Values,
                                     Predicate<V> arg2Predicate,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate,
                                     unordered_set<V> arg2Values,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Find R(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(K arg1, Predicate<V> arg2Predicate,
                                     QueryResultBuilder<K, V> *) const = 0;

  /**
   * Find R(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate, V arg2,
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
