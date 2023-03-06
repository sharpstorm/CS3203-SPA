#pragma once
#include <memory>
#include <vector>

enum CFGNodeType {
  CFG_STARTING_BLOCK,
  CFG_NORMAL_BLOCK,
  CFG_BRANCHING_BLOCK,
  CFG_ENDING_BLOCK
};

typedef shared_ptr<CFGNode> CFGNodePtr;

using std::vector, std::shared_ptr;

class CFGNode {
 public:
  explicit CFGNode(CFGNodeType type);
  CFGNode(CFGNodeType type, int num);
  void addChild(CFGNodePtr node);
  void setChild(int index, CFGNodePtr node);
  shared_ptr<CFGNode> getChild(int index);
  CFGNodeType getType();

 protected:
  CFGNodeType type;
  int lineNumber;
  vector<CFGNodePtr> children;
};