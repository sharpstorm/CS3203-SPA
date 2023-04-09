#pragma once

#include <vector>

#include "common/Types.h"
#include "common/data_structs/BitField.h"
#include "common/cfg/CFGLinks.h"
#include "common/cfg/CFG.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

typedef QueryResultSet<StmtValue> AffectsResult;
typedef vector<AffectsResult> AffectsResultList;
typedef StmtValueList StmtValueStack;
template<class T>
using LinkerStepGetter = const AffectsResult(*)(
    T *querier, const StmtValue &stmt);
template<class T>
using LinkerResultCallback = bool (*)(T *dfsClosure, const StmtValue &stmt);

template<class CallbackState, class StepGetterState>
class AffectsTLinker {
 private:
  StepGetterState *stepState;
  CallbackState *callState;
  CFG *cfg;

 public:
  AffectsTLinker(StepGetterState *stepState, CallbackState *callState,
                 CFG *cfg) :
      stepState(stepState), callState(callState), cfg(cfg) {}

  void linkFrom(LinkerResultCallback<CallbackState> callback,
                LinkerStepGetter<StepGetterState> stepGetter,
                const StmtValue &start) {
    StmtValueStack dfsStack;
    BitField seen(cfg->getNodeCount());
    dfsStack.push_back(start);
    while (!dfsStack.empty()) {
      StmtValue curStmt = dfsStack.back();
      dfsStack.pop_back();

      AffectsResult affectsResult = stepGetter(stepState, curStmt);
      for (const StmtValue &stmt : affectsResult) {
        CFGNode nextNode = cfg->toCFGNode(stmt);
        if (seen.isSet(nextNode)) {
          continue;
        }
        if (!callback(callState, stmt)) {
          return;
        }
        seen.set(nextNode);
        dfsStack.push_back(stmt);
      }
    }
  }
};
