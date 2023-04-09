#pragma once

#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/writers/CFGResultWriterFactory.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
class CFGNextTQuerier : public CFGQuerier<
    CFGNextTQuerier<ClosureType, typePredicate>> {
 private:
  CFG *cfg;
  CFGWalker walker;
  const ClosureType &closure;

  void queryForward(const StmtValue &arg0, ICFGWriter *writer) {
    constexpr WalkerSingleCallback<ICFGWriter> callback =
        [](ICFGWriter *writer, CFGNode node) {
          StmtValue stmtNumber = writer->toStmtNumber(node);
          return writer->writeRight(stmtNumber);
        };

    CFGNode nodeStart = cfg->toCFGNode(arg0);
    walker.walkFrom<ICFGWriter, callback>(nodeStart, writer);
  }

 public:
  CFGNextTQuerier(CFG *cfg, const ClosureType &closure) :
      cfg(cfg), walker(cfg), closure(closure) {}

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) {
    StmtTransitiveResult result;

    if (!cfg->containsStatement(arg0)
        || !cfg->containsStatement(arg1)) {
      return result;
    }

    CFGNode nodeStart = cfg->toCFGNode(arg0);
    CFGNode nodeEnd = cfg->toCFGNode(arg1);

    if (walker.walkStatic(nodeStart, nodeEnd)) {
      result.setNotEmpty();
    }

    return result;
  }

  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1) {
    StmtTransitiveResult result;
    if (!cfg->containsStatement(arg0)) {
      return result;
    }

    ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &closure, &result)
        .makeRightWriter<typePredicate>(arg0, type1);
    queryForward(arg0, writer.get());
    return result;
  }

  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1) {
    StmtTransitiveResult result;
    if (!cfg->containsStatement(arg1)) {
      return result;
    }

    ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &closure, &result)
        .makeLeftWriter<typePredicate>(type0, arg1);

    constexpr WalkerSingleCallback<ICFGWriter> callback =
        [](ICFGWriter *writer, CFGNode node) {
          StmtValue stmtNumber = writer->toStmtNumber(node);
          return writer->writeLeft(stmtNumber);
        };

    CFGNode nodeEnd = cfg->toCFGNode(arg1);
    walker.walkTo<ICFGWriter, callback>(nodeEnd, writer.get());

    return result;
  }

  void queryAll(StmtTransitiveResult *resultOut, const StmtType &type0,
                const StmtType &type1) {
    ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &closure, resultOut)
        .makePairWriter<typePredicate>(0, type0, type1);
    for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
      StmtValue stmtNumber = cfg->fromCFGNode(start);
      writer->setLeft(stmtNumber);

      if (!typePredicate(closure, type0, stmtNumber)) {
        continue;
      }

      queryForward(stmtNumber, writer.get());
    }
  }
};
