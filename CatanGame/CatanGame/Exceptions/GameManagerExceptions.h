#pragma once

#include "Exceptions/Exceptions.h"

INHERIT_EXCEPTION(GameManagerError, CatanException)

INHERIT_EXCEPTION(InvalidPlayersNumber, GameManagerError)

INHERIT_EXCEPTION(InvalidPlaceForSettlement, GameManagerError)

INHERIT_EXCEPTION(InvalidPlaceForEdge, GameManagerError)