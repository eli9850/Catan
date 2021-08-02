// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include "Utils/MapUtils.h"
#include "Utils/StringUtils.h"
#include "WinWrapers/WinUtils.h"
#include "WinWrapers/Events/Event.h"
#include "WinWrapers/Socket/Socket.h"
#include "WinWrapers/WinUtils.h"
#include "WaitQueue/WaitQueue.h"

#include <WinSock2.h>
#include <Windows.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <mutex>

#endif //PCH_H
