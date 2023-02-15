#include "StatementResult.h"

bool StatementResult::isEmpty() {
  return lines.empty() && linePairs.empty();
}
bool StatementResult::operator==(StatementResult sr) const {
  bool linesEqual = lines == sr.lines;
  bool linePairsEqual = linePairs == sr.linePairs;
  bool isLeftArgEqual = isLeftArg == sr.isLeftArg;
  return linesEqual && linePairsEqual && isLeftArgEqual;
}
