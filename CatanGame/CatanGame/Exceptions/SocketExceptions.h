#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(SocketError, CatanException)

INHERIT_EXCEPTION(FailedInitializeSocket, SocketError)