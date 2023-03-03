#pragma once

#include <memory>
#include <utility>

#include "ProcedureContextType.h"
#include "../SubContextParser.h"
#include "ProcedureContextProvider.h"

using std::unique_ptr, std::make_unique;

typedef SubContextParser<ProcedureContextType,
                         ProcedureContextType::PROCEDURE_CONTEXT>
    IProcedureParser;

class ProcedureParser: public IProcedureParser {
 public:
  ProcedureParser(IEntityParser* entityParser,
                  IExpressionParser* exprParser,
                  IConditionalParser* condParser):
                  SubContextParser(make_unique<ProcedureContextProvider>(
                      entityParser, exprParser, condParser)) {}
};

typedef unique_ptr<IProcedureParser> IProcedureParserPtr;
