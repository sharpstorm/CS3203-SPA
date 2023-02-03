#pragma once

#include <string>
#include "../storage/StatementStorage.h"
#include "../../common/Types.h"
class StatementWriter {
 public:
    explicit StatementWriter(StatementStorage* statementStorage);
    void addStatement(int , StmtType);

 private:
    StatementStorage* statementStorage;
};
