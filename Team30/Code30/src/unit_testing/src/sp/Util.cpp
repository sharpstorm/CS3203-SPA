#include <string>
#include <utility>
#include <vector>

#include "catch.hpp"
using std::vector, std::string, std::pair;

class Util {
 public:
  Util() {}

  bool contains(vector<pair<int, int>> v, int firstLineNumber,
                int secondLineNumber) {
    for (pair<int, int> p : v) {
      if (p.first == firstLineNumber && p.second == secondLineNumber) {
        return true;
      }
    }
    return false;
  }

  bool contains(vector<pair<int, string>> v, int lineNumber, string var) {
    for (pair<int, string> p : v) {
      if (p.first == lineNumber && p.second == var) {
        return true;
      }
    }
    return false;
  }

  bool isSize(vector<pair<int, int>> v, int size) { return v.size() == size; }

  bool isSize(vector<pair<int, string>> v, int size) {
    return v.size() == size;
  }
};
