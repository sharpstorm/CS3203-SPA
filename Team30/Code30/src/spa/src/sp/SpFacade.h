#pragma onceonce
#include <string>
#include "ISp.h"

using std::string;

class SpFacade : public ISp {
 public:
  void parseSource(string input);
};
