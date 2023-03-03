#pragma once

class IPostProcessWriter {
 public:
  virtual ~IPostProcessWriter() {}
  /**
   * To be called after SP parses source code into PKB.
   */
  virtual void runPostProcessor() = 0;
};
