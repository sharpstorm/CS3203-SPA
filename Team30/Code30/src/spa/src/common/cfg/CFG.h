#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "common/UtilityTypes.h"
#include "CFGLinks.h"

using std::string, std::vector, std::shared_ptr;

// CFG can only handle up to 65534 Nodes
class CFG {
 public:
  static const int MASK_LOWER_16 = 0xFFFF;
  explicit CFG(const int &start);

  bool containsStatement(const int &stmtNo);
  bool containsNode(const CFGNode &node);

  int getStartingStmtNumber();
  int getNodeCount();
  CFGNode toCFGNode(const int &stmtNo);
  int fromCFGNode(const CFGNode &node);

  // All CFG Operations operate on an internal index
  void addLink(const CFGNode &from, const CFGNode &to);

  CFGLinks* nextLinksOf(const CFGNode& node);
  CFGLinks* reverseLinksOf(const CFGNode& node);

 private:
  int startingLineIndex;

  vector<CFGLinks> forwardLinks;
  vector<CFGLinks> backwardLinks;
  CFGLinks endNodeBackwardLink;

  void increaseMapSize(int num);

  template <class T>
  constexpr void addLink(T* target, const int &from, const int &to) {
    if (from == CFG_END_NODE) {
      return;
    }

    if (target->size() < from + 1) {
      increaseMapSize(from + 1);
    }

    target->at(from).addLink(to);
  }
};

typedef shared_ptr<CFG> CFGSPtr;
