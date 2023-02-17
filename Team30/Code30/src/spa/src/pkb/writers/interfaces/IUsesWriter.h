#pragma once

#include <string>

using std::string;

class IUsesWriter {
 public:
  virtual ~IUsesWriter() {}
  /**
   * Adds relation where statement directly uses variable.
   */
  virtual void addUses(int stmtNum, string variable) = 0;
};
