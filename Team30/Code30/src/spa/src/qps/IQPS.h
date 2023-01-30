#pragma once

#include <string>

using std::string;

class IQPS {
 public:
    virtual string* evaluate(string query);
};
