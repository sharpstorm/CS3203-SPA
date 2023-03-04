#include <string>
#include <utility>
#include <vector>

#include "catch.hpp"
using std::vector, std::string, std::pair;

template<typename T1, typename T2, typename T3>
using triple = std::tuple<T1, T2, T3>;

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

  bool contains(vector<pair<string, string>> v, string s1,
                string s2) {
    for (pair<string, string> p : v) {
      if (p.first == s1 && p.second == s2) {
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

  bool contains(vector<triple<string, int, int>> v,
                string name, int start, int end) {
    for (triple<string, int, int> p : v) {
      if (std::get<0>(p) == name && std::get<1>(p) == start &&
          std::get<2>(p) == end) {
        return true;
      }
    }
    return false;
  }

  bool isSize(vector<pair<int, int>> v, int size) {
    return v.size() == size;
  }

  bool isSize(vector<pair<int, string>> v, int size) {
    return v.size() == size;
  }
};
