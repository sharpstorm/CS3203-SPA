#pragma once

#include <memory>

#include "Clause.h"

using std::unique_ptr;

class SuchThatClause: virtual public Clause {
};

typedef unique_ptr<SuchThatClause> SuchThatClausePtr;
