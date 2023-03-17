#include "TestPipelineProvider.h"
#include "qps/QPSFacade.h"
#include "../TestUtils.h"

TestPipelineProvider::TestPipelineProvider():
    pkb(make_unique<PKB>()),
    pkbWriter(make_unique<PkbWriter>(pkb.get())),
    pkbQH(make_unique<PkbQueryHandler>(pkb.get())),
    qps(make_unique<QPSFacade>(pkbQH.get(), &spDriver)) {
  spDriver.parseSource(SOURCE1, pkbWriter.get());
}

void TestPipelineProvider::query(const string &query,
                                 const unordered_set<string> &answer) {
  launchQuery(qps.get(), query, answer);
}

void TestPipelineProvider::expectSyntaxError(const string &query) {
  launchSyntaxErrorQuery(qps.get(), query);
}

void TestPipelineProvider::expectSemanticError(const string &query) {
  launchSemanticErrorQuery(qps.get(), query);
}
