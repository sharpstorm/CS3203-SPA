#include <vector>

#include "catch.hpp"

using std::vector;

class TestUtils {
 public:
  template <class T>
  static bool vectorContains(vector<T> container, T target) {
    for (T item : container) {
      if (item == target) {
        return true;
      }
    }
    return false;
  }
};
