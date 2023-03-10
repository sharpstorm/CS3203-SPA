#pragma once

#include "WithArgument.h"

class WithArgumentFactory {
 public:
  static WithArgumentPtr create(int intVal);
  static WithArgumentPtr create(string ident);
  static WithArgumentPtr create(AttributedSynonym syn);
};