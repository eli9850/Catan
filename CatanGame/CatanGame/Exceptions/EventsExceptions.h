#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(EventExcption, CatanException)

INHERIT_EXCEPTION(CreateEventException, EventExcption)

INHERIT_EXCEPTION(SetEventException, EventExcption)

INHERIT_EXCEPTION(ResetEventException, EventExcption)