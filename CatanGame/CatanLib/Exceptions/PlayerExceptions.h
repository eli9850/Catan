#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(PlayerException, CatanException)

INHERIT_EXCEPTION(WrongNumberOfSlashResourceException, PlayerException)
