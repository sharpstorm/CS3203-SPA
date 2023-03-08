#pragma once

#include "IPostValidator.h"
#include "pkb/storage/PKB.h"

class CyclicProceduresValidator : public IPostValidator {
 public:
  explicit CyclicProceduresValidator(PKB *);

  void validate() override;

 private:
  PKB *pkb;
};
