#pragma once

#include "qps/IQPS.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/errors/QPSParserSyntaxError.h"

#include <string>
#include <unordered_set>

using std::string, std::unordered_set, std::to_string;

void launchQuery(IQPS* qps, string query, unordered_set<string> answer);
void launchSyntaxErrorQuery(IQPS* qps, string query);
void launchSemanticErrorQuery(IQPS* qps, string query);

void assertSetEquality(unordered_set<string> a, unordered_set<string> b);
