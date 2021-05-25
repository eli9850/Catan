#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(BoardError, CatanException)

INHERIT_EXCEPTION(InvalidEdgeIndex, BoardError)

INHERIT_EXCEPTION(InvalidNodeIndex, BoardError)

INHERIT_EXCEPTION(InvalidResourceIndex, BoardError)

INHERIT_EXCEPTION(NotSettelemnt, BoardError)
