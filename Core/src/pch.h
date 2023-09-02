#pragma once

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <optional>
#include <exception>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Core/Log.h"

#include "Memory/MemoryLib.h"

struct WindowInfo {
    int width;
    int height;
    HWND nativeWnd;
    bool windowed;
};

#include "Platform/WindowsInclude.h"

#endif //PCH_H
