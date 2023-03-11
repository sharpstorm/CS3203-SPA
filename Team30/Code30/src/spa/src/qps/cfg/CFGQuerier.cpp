#include "CFGQuerier.h"

CFGQuerier::CFGQuerier(CFG *cfg): cfg(cfg) {
}

StmtTransitiveResult CFGQuerier::queryNext(const StmtRef &arg0,
                                           const StmtRef &arg1) {
  if (arg0.isKnown() && arg1.isKnown()) {
    return queryBool(arg0.lineNum, arg1.lineNum);
  } else if (arg0.isKnown()) {
    return queryFrom(arg0.lineNum);
  } else if (arg1.isKnown()) {
    return queryTo(arg1.lineNum);
  }
  return queryAll();
}

StmtTransitiveResult CFGQuerier::queryBool(const StmtValue &arg0,
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

StmtTransitiveResult CFGQuerier::queryFrom(const StmtValue &arg0) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGForwardLink* links = cfg->nextLinksOf(nodeFrom);
  for (auto it = links->begin(); it != links->end(); it++) {
    result.add(arg0, cfg->fromCFGNode(*it));
  }

  return result;
}

StmtTransitiveResult CFGQuerier::queryTo(const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  CFGBackwardLink* links = cfg->reverseLinksOf(nodeTo);
  for (auto it = links->begin(); it != links->end(); it++) {
    result.add(cfg->fromCFGNode(*it), arg1);
  }

  return result;
}

StmtTransitiveResult CFGQuerier::queryAll() {
  return StmtTransitiveResult();
}
