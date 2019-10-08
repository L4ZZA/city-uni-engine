// --------- Entry Point ---------------
#include <engine/core/entry_point.h>
#include "sanbox_app.h"


//--------------------------------------------------------------

engine::application* engine::create_application() 
{ 
    return new sandbox_app(); 
}

//--------------------------------------------------------------
