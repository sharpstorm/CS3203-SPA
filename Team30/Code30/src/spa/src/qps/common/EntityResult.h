#pragma once

#include <string>
#include <unordered_set>
#include "common/Types.h"

using std::string, std::unordered_set;

class EntityResult {
 public:
  unordered_set<int> lines;
  unordered_set<string> entities;
  pair_set<int, string> entityPairs;
  unordered_set<pair<string, string>> procedurePairs;
  bool isLeftArg;
  bool isEmpty();
  bool operator ==(EntityResult er) const;
};
