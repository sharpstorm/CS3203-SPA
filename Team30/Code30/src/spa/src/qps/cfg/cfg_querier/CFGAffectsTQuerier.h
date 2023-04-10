#pragma once

#include <vector>
#include <unordered_set>

#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/walkers/CFGStatefulWalker.h"
#include "qps/cfg/cfg_querier/CFGQuerier.h"
#include "common/SetUtils.h"
#include "CFGAffectsQuerier.h"
#include "qps/cfg/cfg_querier/writers/CFGResultWriterFactory.h"
#include "AffectsResultTable.h"
#include "AffectsTLinker.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class ConcreteAffectsQuerier>
class CFGAffectsTQuerier :
    public CFGQuerier<CFGAffectsTQuerier<ConcreteAffectsQuerier>> {
  typedef AffectsTLinker<ICFGWriter, ConcreteAffectsQuerier> DefaultLinker;

 public:
  explicit CFGAffectsTQuerier(CFG *cfg,
                              const ConcreteAffectsQuerier &affectsQuerier);

  void queryBool(StmtTransitiveResult *result, const StmtValue &arg0,
                 const StmtValue &arg1);
  void queryFrom(StmtTransitiveResult *result, const StmtValue &arg0,
                 const StmtType &type1);
  void queryTo(StmtTransitiveResult *result, const StmtType &type0,
               const StmtValue &arg1);
  void queryAll(StmtTransitiveResult *resultOut, const StmtType &type0,
                const StmtType &type1);

  template<class T>
  static constexpr TypePredicate<T> dummyTypePredicate =
      [](const T &state, StmtType type, StmtValue value) -> bool {
        return true;
      };

 private:
  CFG *cfg;
  ConcreteAffectsQuerier affectsQuerier;

  static constexpr LinkerStepGetter<ConcreteAffectsQuerier> affectsQueryFrom =
      [](ConcreteAffectsQuerier *querier,
         const StmtValue &stmt) -> const AffectsResult {
        StmtTransitiveResult result;
        querier->queryFrom(&result, stmt, StmtType::Assign);
        return result.getRightVals();
      };

  static constexpr LinkerStepGetter<ConcreteAffectsQuerier> affectsQueryTo =
      [](ConcreteAffectsQuerier *querier,
         const StmtValue &stmt) -> const AffectsResult {
        StmtTransitiveResult result;
        querier->queryTo(&result, StmtType::Assign, stmt);
        return result.getLeftVals();
      };
};

template<class ConcreteAffectsQuerier>
CFGAffectsTQuerier<ConcreteAffectsQuerier>::CFGAffectsTQuerier(
    CFG *cfg, const ConcreteAffectsQuerier &closure):
    cfg(cfg), affectsQuerier(closure) {}

template<class ConcreteAffectsQuerier>
void CFGAffectsTQuerier<ConcreteAffectsQuerier>::queryBool(
    StmtTransitiveResult *result, const StmtValue &arg0,
    const StmtValue &arg1) {
  if (!affectsQuerier.validateArg(arg0) || !affectsQuerier.validateArg(arg1)) {
    return;
  }

  ICFGWriterPtr writer = CFGResultWriterFactory(cfg, cfg, result)
      .template makeBoolWriter<dummyTypePredicate<CFG>>(arg0, arg1);
  constexpr LinkerResultCallback<ICFGWriter> resultHandler =
      [](ICFGWriter *writer, const StmtValue &stmt) -> bool {
        return writer->writeBool(stmt);
      };

  DefaultLinker linker(&affectsQuerier, writer.get(), cfg);
  linker.linkFrom(resultHandler, affectsQueryFrom, arg0);
}

template<class ConcreteAffectsQuerier>
void CFGAffectsTQuerier<ConcreteAffectsQuerier>::queryFrom(
    StmtTransitiveResult *result, const StmtValue &arg0,
    const StmtType &type1) {
  if (!affectsQuerier.validateArg(arg0)) {
    return;
  }

  ICFGWriterPtr writer = CFGResultWriterFactory(cfg, cfg, result)
      .template makeRightWriter<dummyTypePredicate<CFG>>(arg0, type1);
  constexpr LinkerResultCallback<ICFGWriter> resultHandler =
      [](ICFGWriter *writer, const StmtValue &stmt) -> bool {
        return writer->writeRight(stmt);
      };

  DefaultLinker linker(&affectsQuerier, writer.get(), cfg);
  linker.linkFrom(resultHandler, affectsQueryFrom, arg0);
}

template<class ConcreteAffectsQuerier>
void CFGAffectsTQuerier<ConcreteAffectsQuerier>::queryTo(
    StmtTransitiveResult *result, const StmtType &type0,
    const StmtValue &arg1) {
  if (!affectsQuerier.validateArg(arg1)) {
    return;
  }

  ICFGWriterPtr writer = CFGResultWriterFactory(cfg, cfg, result)
      .template makeLeftWriter<dummyTypePredicate<CFG>>(type0, arg1);
  constexpr LinkerResultCallback<ICFGWriter> resultHandler =
      [](ICFGWriter *writer, const StmtValue &stmt) -> bool {
        return writer->writeLeft(stmt);
      };

  DefaultLinker linker(&affectsQuerier, writer.get(), cfg);
  linker.linkFrom(resultHandler, affectsQueryTo, arg1);
}

template<class ConcreteAffectsQuerier>
void CFGAffectsTQuerier<ConcreteAffectsQuerier>::
queryAll(StmtTransitiveResult *resultOut,
         const StmtType &type0,
         const StmtType &type1) {
  using ResultTable = AffectsResultTable<ConcreteAffectsQuerier>;
  using SpecialLinker = AffectsTLinker<ICFGWriter, ResultTable>;

  ResultTable affectsResults(cfg, &affectsQuerier, cfg->getNodeCount());
  constexpr LinkerResultCallback<ICFGWriter> resultHandler =
      [](ICFGWriter *writer, const StmtValue &stmt) -> bool {
        return writer->writeRight(stmt);
      };

  constexpr LinkerStepGetter<ResultTable> stepGetter =
      [](ResultTable *table, const StmtValue &stmt) -> const AffectsResult {
        return table->getResultAt(stmt);
      };

  ICFGWriterPtr writer =
      CFGResultWriterFactory(cfg, &affectsResults, resultOut)
          .template makePairWriter<dummyTypePredicate<ResultTable>>(0,
                                                                    type0,
                                                                    type1);

  for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
    StmtValue startStmt = cfg->fromCFGNode(start);
    writer->setLeft(startStmt);
    SpecialLinker linker(&affectsResults, writer.get(), cfg);
    linker.linkFrom(resultHandler, stepGetter, startStmt);

    if (type0 == StmtType::Wildcard && type1 == StmtType::Wildcard
        && !resultOut->empty()) {
      break;
    }
  }
}
