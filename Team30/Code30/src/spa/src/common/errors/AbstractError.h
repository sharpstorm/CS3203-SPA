#pragma once

#include <string>

using std::string;

class AbstractError : public std::exception {
 private:
  const string errMsg;

 protected:
  explicit AbstractError(const string &message);

 public:
  const char* what() const noexcept override;
};
