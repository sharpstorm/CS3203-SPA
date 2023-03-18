#include <memory>

#include "catch.hpp"
#include "common/cfg/CFG.h"
#include "pkb/writers/CFGsWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("CFGsWriter addCFGs") {
  auto store = make_unique<CFGStorage>();
  auto writer = CFGsWriter(store.get());

  auto CFGRoot = make_shared<CFG>(1);
  CFGRoot->addLink(0, 1);
  CFGRoot->addLink(1, 2);
  CFGRoot->addLink(2, 3);
  CFGRoot->addLink(3, CFG_END_NODE);

  writer.addCFGs("main", CFGRoot);

  REQUIRE(store->get("main").get() == CFGRoot.get());
}
