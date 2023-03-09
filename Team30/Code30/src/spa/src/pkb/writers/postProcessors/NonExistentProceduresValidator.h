#pragma once

#include "IPostValidator.h"
#include "pkb/storage/PKB.h"

class NonExistentProceduresValidator : public IPostValidator {
 public:
  explicit NonExistentProceduresValidator(PKB *);

  void validate() override;

 private:
  PKB *pkb;
};
