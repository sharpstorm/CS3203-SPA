#include "QueryGroupOrderer.h"

#include <utility>
#include <vector>
#include <algorithm>

using std::vector;

QueryGroupPlanPtrList QueryGroupOrderer::orderGroups(
    QueryGroupPlanPtrList unordered) const {
  vector<ComparableGroup> groups;

  for (size_t i = 0; i < unordered.size(); i++) {
    groups.push_back(ComparableGroup(unordered.at(i)->getComplexity(), i));
  }

  std::sort(groups.begin(), groups.end());
  QueryGroupPlanPtrList result;

  for (size_t i = 0; i < groups.size(); i++) {
    GroupId target = groups.at(groups.size() - 1 - i).getData();
    result.push_back(std::move(unordered.at(target)));
  }

  return result;
}
