#pragma once

#include <vector>

#include "interfaces/IPostProcessWriter.h"
#include "pkb/writers/postProcessors/IPostProcessor.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/interfaces/IPkbWriter.h"

using std::vector;

class PostProcessWriter : public IPostProcessWriter {
 public:
  explicit PostProcessWriter(PKB *);

  void runPostProcessor() override;

  void addProcessor(IPostProcessor *);

 private:
  vector<IPostProcessor *> processors;
};
