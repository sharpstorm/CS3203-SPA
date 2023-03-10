#include <memory>

#include "WithArgumentFactory.h"

using std::make_unique;

WithArgumentPtr WithArgumentFactory::create(int intVal) {
  return make_unique<WithArgument>(intVal);
}

WithArgumentPtr WithArgumentFactory::create(string ident) {
  return make_unique<WithArgument>(ident);
}

WithArgumentPtr WithArgumentFactory::create(AttributedSynonym syn) {
  return make_unique<WithArgument>(syn);
}
