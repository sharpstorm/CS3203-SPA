#pragma once

#include <string>
#include <unordered_set>
#include <utility>
#include "common/Types.h"

using std::string, std::unordered_set, std::hash;

class EntityResult {
 public:
  unordered_set<int> lines;
  unordered_set<string> entities;
  pair_set<int, string> entityPairs;
  pair_set<string, string> procedurePairs;
  bool isLeftArg;
  bool isEmpty();
  bool operator ==(EntityResult er) const;
};
