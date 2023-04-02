#pragma once

#include "common/Types.h"
#include "pkb/PkbTypes.h"

using pkb::Predicate;

template <typename K, typename V>
class IStorage {
 public:
  virtual ~IStorage() = default;

  virtual void insert(K arg1, V arg2) = 0;

  /**
   * Find R(arg1, arg2) where arg1 and arg2 are given values.
   */
  virtual QueryResultPtr<K, V> query(K arg1, V arg2) const = 0;

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(set<K> arg1Values,
                                     Predicate<V> arg2Predicate) const = 0;

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate,
                                     set<V> arg2Values) const = 0;

  /**
   * Find R(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(K arg1,
                                     Predicate<V> arg2Predicate) const = 0;

  /**
   * Find R(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate,
                                     V arg2) const = 0;

  /**
   * Return non-empty result if at least one such relation
   */
  virtual QueryResultPtr<K, V> hasRelation() const = 0;

  /**
   * Right side wildcard.
   */
  virtual QueryResultPtr<K, V> rightWildcardQuery(
      const set<K> &arg1Values) const = 0;

  /**
   * Left side wildcard.
   */
  virtual QueryResultPtr<K, V> leftWildcardQuery(
      const set<V> &arg2Values) const = 0;
};
