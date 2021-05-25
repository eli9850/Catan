#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(CatanServerError, CatanException)

INHERIT_EXCEPTION(InvalidSocket, CatanServerError)

INHERIT_EXCEPTION(FailedInitializeWinsock, CatanServerError)

INHERIT_EXCEPTION(BindError, CatanServerError)

INHERIT_EXCEPTION(ListenError, CatanServerError)

INHERIT_EXCEPTION(AcceptError, CatanServerError)

INHERIT_EXCEPTION(SendError, CatanServerError)

INHERIT_EXCEPTION(ReciveError, CatanServerError)

INHERIT_EXCEPTION(ConnectError, CatanServerError)
