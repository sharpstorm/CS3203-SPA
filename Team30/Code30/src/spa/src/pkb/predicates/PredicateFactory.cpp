#include "PredicateFactory.h"


PredicateFactory::PredicateFactory() {}

template <typename V, typename T>
Predicate<V> PredicateFactory::getPredicate(
    IRef<T, V> *ref, IProvider<V, T> *provider) const {
  if (ref->isKnown()) {
    return [ref](V const s) { return s == ref->getValue(); };
  } else if (ref->getType() == T()) {
    return [](V const s) { return true; };
  }
  return [this, ref, &provider](V const s) {
    return provider->isValueOfType(s, ref->getType());
  };
}
