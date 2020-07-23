#include "sanbox_app.h"
#include "main_layer.h"

sandbox_app::sandbox_app() 
{
    engine::audio_manager::instance()->init(); 
    push_layer(new example_layer()); 
}

void sandbox_app::on_event(engine::event& event)
{ 
    application::on_event(event); 

    engine::event_dispatcher dispatcher(event); 
    // dispatch event on window X pressed 
    dispatcher.dispatch<engine::key_pressed_event>(BIND_EVENT_FN(sandbox_app::on_key_pressed)); 
} 

bool sandbox_app::on_key_pressed(engine::key_pressed_event& event)
{ 
    if (event.event_type() == engine::event_type_e::key_pressed) 
    { 
        if (event.key_code() == engine::key_codes::KEY_ESCAPE) 
        { 
            application::exit();
        } 
        //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    } 
    return false; 
}
