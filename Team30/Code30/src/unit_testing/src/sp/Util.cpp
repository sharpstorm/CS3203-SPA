#include "catch.hpp"
#include <string>
#include <utility>
#include <vector>
using std::vector, std::string, std::pair;

class Util {
 public:
  Util() {}

  bool contains(vector<pair<int, string>> v, int lineNumber, string var) {
    for (pair<int, string> p : v) {
      if (p.first == lineNumber && p.second == var) {
        return true;
      }
    }
    return false;
  }

  bool isSize(vector<pair<int, string>> v, int size) {
    REQUIRE(v.size() == size);
  }
};

