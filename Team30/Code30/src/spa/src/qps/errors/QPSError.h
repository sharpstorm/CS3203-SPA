#pragma once

class QPSError : public std::exception {
 protected:
  explicit QPSError(const char* message): exception(message) {}
};
