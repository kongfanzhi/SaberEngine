#pragma once

// 标准库
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Saber/Core/Log.h"
#include "Saber/Debug/Instrumentor.h"

// 平台特定
#ifdef _WIN32
#include <Windows.h>
#endif