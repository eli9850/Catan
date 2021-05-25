#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(StructureError, CatanException)

INHERIT_EXCEPTION(UpgradeSettlementError, StructureError)
