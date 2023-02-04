#pragma once

#include <string>
#include "../storage/ProcedureStorage.h"
#include "../../common/Types.h"
class ProcedureWriter {
 public:
    explicit ProcedureWriter(ProcedureStorage* procedureStorage);
    void addProcedure(const std::string& , int, int);

 private:
    ProcedureStorage* procedureStorage;
};
