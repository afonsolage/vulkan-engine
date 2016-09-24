// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <cstdio>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>
#include <fstream>
#include <unordered_map>


#include "CommonUtils.h"

using namespace std::chrono_literals;


#define SAFE_GET(v,m) auto v = m.lock(); \
						if (v == nullptr) \
						std::runtime_error("Invalid "#m"!")

#define GET_CONTEXT SAFE_GET(context, m_context)
#define GET_SWAPCHAIN SAFE_GET(swapchain, m_swapchain)
