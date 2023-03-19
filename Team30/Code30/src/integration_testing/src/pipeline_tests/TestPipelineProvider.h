#pragma once

#include <memory>
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "sp/SpDriver.h"
#include "qps/IQPS.h"

using std::unique_ptr;

const char SOURCE1[] = "procedure Example {\n"
                       "  x = 2;\n" // 1
                       "  z = 3;\n" // 2
                       "  i = 5;\n" // 3
                       "  while (i!=0) {\n" // 4
                       "    x = x - 1;\n" // 5
                       "    if (x==1) then {\n" // 6
                       "      z = x + 1; }\n" // 7
                       "    else {\n"
                       "      y = z + x; }\n" // 8
                       "    z = z + x + i;\n" // 9
                       "    read x;\n" // 10
                       "    i = i - 1; }\n" // 11
                       "  z = x + 1; }"; // 12

const char SOURCE2[] = "procedure Example {\n"
                       "  read y;\n" // 1
                       "  read x;\n" // 2
                       "  read x;\n" // 3
                       "  read y;\n" // 4
                       "}";

class TestPipelineProvider {
 public:
  TestPipelineProvider();
  explicit TestPipelineProvider(string source);
  void query(const string &query, const unordered_set<string> &answer);
  void expectSyntaxError(const string &query);
  void expectSemanticError(const string &query);

 private:
  unique_ptr<PKB> pkb;
  unique_ptr<PkbWriter> pkbWriter;
  unique_ptr<PkbQueryHandler> pkbQH;
  SpDriver spDriver;
  unique_ptr<IQPS> qps;
};
