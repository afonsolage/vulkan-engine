// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <cstdio>
#include <cstdarg>
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
#include "Logging.h"

using namespace std::chrono_literals;


#define SAFE_GET(v,m) auto v = m.lock(); \
						if (v == nullptr) \
						std::runtime_error("Invalid "#m"!")

#define GET_CONTEXT SAFE_GET(context, m_context)
#define GET_SWAPCHAIN SAFE_GET(swapchain, m_swapchain)

#ifdef TEST_MODE
#define LOG_INFO(...) // Logging::info(__VA_ARGS__)
#define LOG_WARN(...) // Logging::warn(__VA_ARGS__)
#define LOG_DEBUG(...) // Logging::debug(__VA_ARGS__)
#define LOG_ERROR(...) // Logging::error(__VA_ARGS__)
#define LOG_FATAL(...) // Logging::fatal(__VA_ARGS__)
#define LOG_TRACE(...) // Logging::trace(__VA_ARGS__)
#else
#define LOG_INFO(...) Logging::info(__VA_ARGS__)
#define LOG_WARN(...) Logging::warn(__VA_ARGS__)
#define LOG_DEBUG(...) Logging::debug(__VA_ARGS__)
#define LOG_ERROR(...) Logging::error(__VA_ARGS__)
#define LOG_FATAL(...) Logging::fatal(__VA_ARGS__)
#define LOG_TRACE(...) Logging::trace(__VA_ARGS__)
#endif