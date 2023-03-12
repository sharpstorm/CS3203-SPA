#include "CFGNextQuerier.h"

CFGNextQuerier::CFGNextQuerier(CFG *cfg):
    cfg(cfg) {}

StmtTransitiveResult CFGNextQuerier::queryBool(const StmtValue &arg0,
                                               const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)
      || !cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGNode nodeTo = cfg->toCFGNode(arg1);

  CFGForwardLink* links = cfg->nextLinksOf(nodeFrom);
  if (links == nullptr) {
    return result;
  }

  for (auto it = links->begin(); it != links->end(); it++) {
    if (*it == nodeTo) {
      result.add(arg0, arg1);
      break;
    }
  }

  return result;
}

StmtTransitiveResult CFGNextQuerier::queryFrom(const StmtValue &arg0) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGForwardLink* links = cfg->nextLinksOf(nodeFrom);
  for (auto it = links->begin(); it != links->end(); it++) {
    CFGNode node = *it;
    if (node == CFG_END_NODE) {
      continue;
    }
    result.add(arg0, cfg->fromCFGNode(node));
  }

  return result;
}

StmtTransitiveResult CFGNextQuerier::queryTo(const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  CFGBackwardLink* links = cfg->reverseLinksOf(nodeTo);
  for (auto it = links->begin(); it != links->end(); it++) {
    CFGNode node = *it;
    if (node == CFG_END_NODE) {
      continue;
    }
    result.add(cfg->fromCFGNode(node), arg1);
  }

  return result;
}

StmtTransitiveResult CFGNextQuerier::queryAll() {
  StmtTransitiveResult result;

  for (CFGNode nodeFrom = 0; nodeFrom < cfg->getNodeCount(); nodeFrom++) {
    CFGForwardLink* links = cfg->nextLinksOf(nodeFrom);
    for (auto it = links->begin(); it != links->end(); it++) {
      CFGNode nodeTo = *it;
      if (nodeTo == CFG_END_NODE) {
        continue;
      }
      result.add(cfg->fromCFGNode(nodeFrom),
                 cfg->fromCFGNode(nodeTo));
    }
  }

  return result;
}
