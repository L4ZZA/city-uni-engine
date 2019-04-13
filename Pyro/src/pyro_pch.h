#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "pyro/logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef PYRO_PLATFORM_WIN
    #include <Windows.h>
#endif 