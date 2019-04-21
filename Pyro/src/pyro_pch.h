#pragma once

#include <memory>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

#include <array>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "pyro/logger.h"
#include "pyro/defs.h"

#ifdef PYRO_PLATFORM_WIN
	#include <Windows.h>
#endif 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "pyro/utils/glm_extension.h"
