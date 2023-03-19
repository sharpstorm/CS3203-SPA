#pragma once

#include <string>

using std::string;

class CFGHaltWalkerException : public std::exception {
 public:
  CFGHaltWalkerException() = default;

 public:
  const char* what() const noexcept override {
    return "CFG Walker Halted";
  }
};
