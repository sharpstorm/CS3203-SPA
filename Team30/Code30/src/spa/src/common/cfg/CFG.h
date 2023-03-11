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
  CFG(const string &name, const int &start);
  CFG();

  bool containsStatement(const int &stmtNo);
  bool containsNode(const CFGNode &node);

  int getStartingStmtNumber();
  int getNodeCount();
  CFGNode toCFGNode(const int &stmtNo);
  int fromCFGNode(const CFGNode &node);

  // All CFG Operations operate on an internal index
  void addLink(const CFGNode &from, const CFGNode &to);

  CFGForwardLink* nextLinksOf(const CFGNode& node);
  CFGBackwardLink* reverseLinksOf(const CFGNode& node);

 private:
  string procedureName;
  int startingLineIndex;

  vector<CFGForwardLink> forwardLinks;
  vector<CFGBackwardLink> backwardLinks;
  CFGBackwardLink endNodeBackwardLink;

  void increaseMapSize(int num);

  template <class T>
  constexpr void addLink(T* target, const int &from, const int &to) {
    assert(from != CFG_NO_NODE);

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
