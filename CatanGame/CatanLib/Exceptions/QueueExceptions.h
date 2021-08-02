#pragma once

#include "Exceptions.h"

INHERIT_EXCEPTION(QueueException, CatanException)

INHERIT_EXCEPTION(FrontQueueException, QueueException)

INHERIT_EXCEPTION(PopQueueException, QueueException)
