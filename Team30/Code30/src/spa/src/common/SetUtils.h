#pragma once

#include <memory>
#include <unordered_set>
#include "UtilityTypes.h"
#include "common/data_structs/AppendSet.h"

using std::make_unique, std::unordered_set;

class SetUtils {
 private:
  template<class T>
  static unique_ptr<T> intersectSetInternal(T *s1, T *s2) {
    if (s1 == nullptr) {
      return nullptr;
    } else if (s2 == nullptr) {
      return nullptr;
    }

    auto result = make_unique<T>();
    bool isS1Smaller = s1->size() < s2->size();
    T *smaller = isS1Smaller ? s1 : s2;
    T *bigger = isS1Smaller ? s2 : s1;
    for (auto it = smaller->begin(); it != smaller->end(); it++) {
      if (bigger->find(*it) != bigger->end()) {
        result->insert(*it);
      }
    }

    return result;
  }

 public:
  template<class T>
  static IntersectSetPtr<T> intersectSet(IntersectSet<T> *s1,
                                         IntersectSet<T> *s2) {
    return intersectSetInternal(s1, s2);
  }

  template<class T>
  static AppendOnlySetPtr<T> intersectSet(AppendSet<T> *s1,
                                          AppendSet<T> *s2) {
    return intersectSetInternal(s1, s2);
  }

  template<class T>
  static T firstItemOfSet(const unordered_set<T> &input,
                          const T &zeroValue) {
    for (const T &item : input) {
      return item;
    }

    return zeroValue;
  }

  template<class T>
  static T setContains(const unordered_set<T> &input,
                       const T &target) {
    return input.find(target) != input.end();
  }
};
