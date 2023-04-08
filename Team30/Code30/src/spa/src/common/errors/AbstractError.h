#pragma once

#include <string>

using std::string;

typedef string ErrorMessage;

class AbstractError : public std::exception {
 private:
  const ErrorMessage errMsg;

 protected:
  explicit AbstractError(const ErrorMessage &message);

 public:
  const char* what() const noexcept override;
};
