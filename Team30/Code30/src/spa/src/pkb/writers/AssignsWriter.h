#pragma once

#include <string>
#include <memory>
#include "pkb/storage/StorageTypes.h"
#include "pkb/writers/interfaces/IAssignsWriter.h"

using std::string;

class AssignsWriter : public IAssignsWriter {
 public:
  AssignsWriter();

  void addAssigns(int, shared_ptr<IASTNode>) override;
};
