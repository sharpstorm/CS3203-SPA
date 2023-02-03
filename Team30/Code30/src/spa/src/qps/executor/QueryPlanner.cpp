#include "QueryPlanner.h"

vector<shared_ptr<IEvaluatable>> QueryPlanner::getExecuteOrder(
    PQLQuery* query) {
    return query->getEvaluatables();
}
