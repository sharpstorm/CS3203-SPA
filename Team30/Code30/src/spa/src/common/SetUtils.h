#pragma once

#include <memory>
#include "UtilityTypes.h"

using std::make_unique;

class SetUtils {
 public:
  template<class T>
  static IntersectSetPtr<T> intersectSet(IntersectSet<T>* s1,
                                                   IntersectSet<T>* s2) {
    if (s1 == nullptr) {
      return nullptr;
    } else if (s2 == nullptr) {
      return nullptr;
    }

    auto result = make_unique<IntersectSet<T>>();
    for (auto it = s1->begin(); it != s1->end(); it++) {
      if (s2->find(*it) != s2->end()) {
        result->insert(*it);
      }
    }

    return result;
  }
};
