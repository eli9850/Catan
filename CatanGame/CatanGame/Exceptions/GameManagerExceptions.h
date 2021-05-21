#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(GameManagerError, CatanException)

INHERIT_EXCEPTION(InvalidPlayersNumber, GameManagerError)

INHERIT_EXCEPTION(UnknownCommand, GameManagerError)