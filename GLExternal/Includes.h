#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Windows.h"
#include "iostream"
#include "string"
#include <Psapi.h>
#include <optional>
#include <map>
#include <thread>
#include <tlhelp32.h>
#include <filesystem>
#include <tchar.h>

#include "Log/Logger.h"
