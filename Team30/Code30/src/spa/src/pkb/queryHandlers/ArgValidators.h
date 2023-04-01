#pragma once

#include "common/Types.h"
#include "pkb/PkbTypes.h"

using pkb::StmtRefValidator, pkb::EntityRefValidator;

constexpr StmtRefValidator modifiesLeftArgValidator =
    [](const IRef<StmtValue, StmtType> *arg) {
      return !arg->isType(StmtType::Print) && !arg->isType(StmtType::Wildcard);
    };

constexpr StmtRefValidator usesLeftArgValidator =
    [](const IRef<StmtValue, StmtType> *arg) {
      return !arg->isType(StmtType::Read) && !arg->isType(StmtType::Wildcard);
    };

// for modifies and uses
constexpr EntityRefValidator varRightArgValidator =
    [](const IRef<EntityValue, EntityType> *arg) {
      return arg->isType(EntityType::None) && arg->isKnown() ||
             arg->isType(EntityType::Variable) ||
             arg->isType(EntityType::Wildcard);
    };

// for modifiesP and usesP
constexpr EntityRefValidator procLeftArgValidator =
    [](const IRef<EntityValue, EntityType> *arg) {
      return arg->getType() == EntityType::Procedure ||
             (arg->getType() == EntityType::None && arg->isKnown());
    };

constexpr EntityRefValidator callsArgValidator =
    [](const IRef<EntityValue, EntityType> *arg) {
      return (arg->isType(EntityType::None) && arg->isKnown()) ||
             arg->isType(EntityType::Procedure) ||
             arg->isType(EntityType::Wildcard);
    };

constexpr StmtRefValidator ifLeftArgValidator =
    [](const IRef<StmtValue, StmtType> *arg) {
      return arg->getType() == StmtType::If ||
             (arg->getType() == StmtType::None && arg->isKnown());
    };

constexpr StmtRefValidator whileLeftArgValidator =
    [](const IRef<StmtValue, StmtType> *arg) {
      return arg->getType() == StmtType::While ||
             (arg->getType() == StmtType::None && arg->isKnown());
    };

constexpr StmtRefValidator parentLeftArgValidator =
    [](const IRef<StmtValue, StmtType> *arg) {
      return arg->isType(StmtType::While) ||
             arg->isType(StmtType::If) ||
             arg->isType(StmtType::None) ||
             arg->isType(StmtType::Wildcard);
    };
