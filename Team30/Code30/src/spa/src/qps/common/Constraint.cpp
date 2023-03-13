#include <utility>

#include "Constraint.h"

Constraint::Constraint(WithArgumentPtr left, WithArgumentPtr right) :
    leftArg(std::move(left)), rightArg(std::move(right)) { }

bool Constraint::isStaticArguments() {
  return !leftArg->isSyn() && !rightArg->isSyn();
}

// TODO(WeiXin): Change this for cat 3
bool Constraint::replaceSyns(VariableTable variables) {
  return false;
}

//TODO(WeiXin): Change the signature for cat 2 (need OverrideTable)
bool Constraint::mapSynToStatic() {
  return false;
}

bool Constraint::applyConstraint(VariableTable variables) {
  if (isStaticArguments()) {
    return leftArg->isStaticValueEqual(*rightArg);
  }

  // TODO(WeiXin): Handle cat 2 & 3 here
}

bool Constraint::validateConstraints() {
  if (!leftArg->isAttributeValid() || !rightArg->isAttributeValid()) {
    return false;
  }
  bool leftRetInt = leftArg->doesReturnInteger();
  bool rightRetInt = rightArg->doesReturnInteger();

  // They both must return an int or a str
  return leftRetInt == rightRetInt;
}
