#pragma once

#include <vector>
#include <memory>

#include "interfaces/IPostProcessWriter.h"
#include "pkb/writers/postProcessors/IPostProcessor.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/interfaces/IPkbWriter.h"

using std::vector, std::unique_ptr;

class PostProcessWriter : public IPostProcessWriter {
 public:
  explicit PostProcessWriter(PKB *);
  void runPostProcessor() override;

 private:
  vector<unique_ptr<IPostProcessor>> processors;
};
