#include "catch.hpp"

#include <memory>

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "WithSelectPKBStub.cpp"

using std::make_unique, std::unique_ptr;

TEST_CASE("WithSelectClause - evaluate (Default Cases)") {
  PKB pkb;
  unique_ptr<PkbQueryHandler> handler = make_unique<WithSelectPKBStub>(&pkb);
}