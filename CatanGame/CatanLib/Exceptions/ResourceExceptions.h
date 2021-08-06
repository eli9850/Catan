#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(ResourceError, CatanException)

INHERIT_EXCEPTION(ResourceDoesNotExists, ResourceError)

INHERIT_EXCEPTION(WrongResourceNumber, ResourceError)
