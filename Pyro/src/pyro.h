#pragma once

// For whoever wants to use Pyro library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "pyro/defs.h"
#include "pyro/core.h"
#include "pyro/application.h"
#include "pyro/layer.h"
#include "pyro/logger.h"

#include "pyro/events/event.h"
#include "pyro/events/application_event.h"
#include "pyro/events/key_event.h"
#include "pyro/events/mouse_event.h"

#include "pyro/key_codes.h"
#include "pyro/mouse_buttons_codes.h"
#include "pyro/input.h"

#include "pyro/graphics/shader.h"
#include "pyro/graphics/model.h"

#include "pyro/graphics/models/raw_model.h"
#include "pyro/graphics/textures/model_texture.h"
#include "pyro/graphics/models/textured_model.h"
#include "pyro/entities/camera.h"
#include "pyro/entities/entity.h"
#include "pyro/entities/light.h"
#include "pyro/graphics/obj_loader.h"
#include "pyro/utils/maths.h"

// ------------- Utils -----------------
#include "pyro/utils/timer.h"


// --------- Entry Point ---------------
//#include "pyro/entry_point.h"