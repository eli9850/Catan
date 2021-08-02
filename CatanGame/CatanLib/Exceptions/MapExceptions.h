#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(MapException, CatanException)

INHERIT_EXCEPTION(SlashFromEmptyValue, MapException)
