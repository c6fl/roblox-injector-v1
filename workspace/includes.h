#pragma once

// windows
#include <Windows.h>
#include <fstream>
#include <string_view>
#include <Psapi.h>
#include <ranges>
#include <algorithm>
#include <vector>
#include <TlHelp32.h>
#include <thread>
#include <iostream>

// own
#include "instrumentations/ic_hook.h"
#include "windows/windows.h"
#include "ntapi/ntapi.h"
#include "utility/utility.h"