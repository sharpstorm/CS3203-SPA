#pragma once

#include <string>

using std::string;

class IModifiesWriter {
 public:
  /**
   * Adds relation where statement directly modifies variable.
   */
  virtual void addModifies(int stmtNum, string variable) = 0;
};
