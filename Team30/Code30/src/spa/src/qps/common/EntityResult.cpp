#include "EntityResult.h"

bool EntityResult::isEmpty() {
  return lines.empty() && entities.empty() && enitityPairs.empty();
}
bool EntityResult::operator==(EntityResult er) const {
  bool linesEqual = lines == er.lines;
  bool entitiesEqual = entities == er.entities;
  bool entityPairsEqual = enitityPairs == er.enitityPairs;
  bool isLeftArgEqual = isLeftArg == er.isLeftArg;

  return linesEqual && entitiesEqual && entityPairsEqual && isLeftArgEqual;
}
