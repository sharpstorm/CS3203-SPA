#pragma once

#include <memory>
#include <optional>

#include "BaseQueryInvoker.h"
#include "pkb/storage/IStorage.h"

using pkb::ArgValidator, pkb::ArgTransformer, pkb::defaultValidator,
    pkb::defaultTransformer;
using std::make_unique;
using std::optional, std::nullopt;

template <typename LeftValue, typename LeftType, typename RightValue,
          typename RightType>
class BaseQueryHandler {
 public:
  explicit BaseQueryHandler(BaseQueryInvoker<LeftValue, LeftType, RightValue,
                                             RightType> *queryInvoker,
                            IStorage<LeftValue, RightValue> *store)
      : queryInvoker(queryInvoker), store(store) {}
  BaseQueryHandler() {}

  QueryResultPtr<LeftValue, RightValue> query(
      IRef<LeftValue, LeftType> *leftArg,
      IRef<RightValue, RightType> *rightArg) const {
    if (!leftValidator(leftArg) || !rightValidator(rightArg)) {
      return make_unique<QueryResult<LeftValue, RightValue>>();
    }

    return queryInvoker->query(store, leftTransformer(leftArg),
                               rightTransformer(rightArg));
  }

 protected:
  void setLeftValidator(ArgValidator<LeftValue, LeftType> validator) {
    leftValidator = validator;
  }

  void setRightValidator(ArgValidator<RightValue, RightType> validator) {
    rightValidator = validator;
  }

  void setLeftTransformer(ArgTransformer<LeftValue, LeftType> transformer) {
    leftTransformer = transformer;
  }

  void setRightTransformer(ArgTransformer<LeftValue, LeftType> transformer) {
    rightTransformer = transformer;
  }

 private:
  BaseQueryInvoker<LeftValue, LeftType, RightValue, RightType> *queryInvoker;
  ArgValidator<LeftValue, LeftType> leftValidator =
      defaultValidator<LeftValue, LeftType>;
  ArgValidator<RightValue, RightType> rightValidator =
      defaultValidator<RightValue, RightType>;
  ArgTransformer<LeftValue, LeftType> leftTransformer =
      defaultTransformer<LeftValue, LeftType>;
  ArgTransformer<RightValue, RightType> rightTransformer =
      defaultTransformer<RightValue, RightType>;
  IStorage<LeftValue, RightValue> *store;
};

using PkbStmtStmtQueryHandler =
    BaseQueryHandler<StmtValue, StmtType, StmtValue, StmtType>;

using PkbStmtEntQueryHandler =
    BaseQueryHandler<StmtValue, StmtType, EntityValue, EntityType>;

using PkbEntEntQueryHandler =
    BaseQueryHandler<EntityValue, EntityType, EntityValue, EntityType>;
