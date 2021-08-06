#pragma once

#include "Exceptions/Exceptions.h"

INHERIT_EXCEPTION(ClientError, CatanException)

INHERIT_EXCEPTION(InvalidSocket, ClientError)

INHERIT_EXCEPTION(FailedInitializeWinsock, ClientError)

INHERIT_EXCEPTION(SendError, ClientError)

INHERIT_EXCEPTION(ReciveError, ClientError)

INHERIT_EXCEPTION(ConnectError, ClientError)
