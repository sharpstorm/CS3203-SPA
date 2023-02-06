#include "FollowsExtractor.h"
#include <iostream>  // TODO(Wei-Xinn) remove this

void FollowsExtractor::visit(StatementListNode node) {
}

void FollowsExtractor::addFollowsRelation(int x, int y) {
  std::cout << x << " : " << y << std::endl;
}
