#pragma once
#include <engine.h>

class sandbox_app : public engine::application 
{ 
public: 
    sandbox_app();
    ~sandbox_app() = default; 

    void on_event(engine::event& event) override;
    bool on_key_pressed(engine::key_pressed_event& event);

};
