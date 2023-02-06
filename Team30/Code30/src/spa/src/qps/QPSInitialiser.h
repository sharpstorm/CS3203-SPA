#include <memory>

#include "IQPS.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::shared_ptr;

class QPSInitialiser {
 public:
  IQPS* initQPS(shared_ptr<PkbQueryHandler> pkbQH);
};
