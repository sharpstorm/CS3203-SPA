#pragma once

#include "pkb/PkbTypes.h"
#include "common/Types.h"

using pkb::StmtRefValidator, pkb::EntityRefValidator;

constexpr StmtRefValidator modifiesLeftArgValidator = [](
    const IRef<StmtValue, StmtType> *arg) {
  return arg->getType() != StmtType::Print;
};

constexpr StmtRefValidator usesLeftArgValidator = [](
    const IRef<StmtValue, StmtType> *arg) {
  return arg->getType() != StmtType::Read;
};

// for modifies and uses
constexpr EntityRefValidator varRightArgValidator = [](
    const IRef<EntityValue, EntityType> *arg) {
  return arg->getType() == EntityType::None
      || arg->getType() == EntityType::Variable;
};

// for modifiesP and usesP
constexpr EntityRefValidator procLeftArgValidator = [](
    const IRef<EntityValue, EntityType> *arg) {
  return arg->getType() == EntityType::Procedure
      || (arg->getType() == EntityType::None && arg->isKnown());
};

constexpr EntityRefValidator callsArgValidator = [](
    const IRef<EntityValue, EntityType> *arg) {
  return arg->getType() == EntityType::None
      || arg->getType() == EntityType::Procedure;
};

constexpr StmtRefValidator ifLeftArgValidator = [](
    const IRef<StmtValue, StmtType> *arg) {
  return arg->getType() == StmtType::If
      || (arg->getType() == StmtType::None && arg->isKnown());
};

constexpr StmtRefValidator whileLeftArgValidator = [](
    const IRef<StmtValue, StmtType> *arg) {
  return arg->getType() == StmtType::While
      || (arg->getType() == StmtType::None && arg->isKnown());
};

constexpr StmtRefValidator parentLeftArgValidator = [](
    const IRef<StmtValue, StmtType> *arg) {
  return arg->getType() == StmtType::While
         || arg->getType() == StmtType::If
         || arg->getType() == StmtType::None
         ||!arg->isKnown();
};
