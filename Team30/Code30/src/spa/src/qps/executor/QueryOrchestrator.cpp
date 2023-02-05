#include "QueryOrchestrator.h"

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
        launcher(launcher) {

}

PQLQueryResult *QueryOrchestrator::execute(PQLQuery* query,
                                           vector<shared_ptr<IEvaluatable>> evaluatables) {
    PQLQueryResult* finalResult = nullptr;
    PQLQueryResult* currentResult;
    for (shared_ptr<IEvaluatable> ie : evaluatables) {
        currentResult = launcher.execute(ie.get());
        finalResult = coalescer.merge(finalResult, currentResult);
    }

    return finalResult->resultFromVariable(query->getResultVariable());
}
