#pragma once
#include "engine/graphics/textures/model_texture.h"
#include "textured_model.h"
#include "raw_model.h"

namespace engine
{
    class textured_model
    {
    public:
        textured_model() = default;
        textured_model(const raw_model &model, const model_texture &texture);
        void create(const raw_model &model, const model_texture &texture);

        raw_model raw() const;
        model_texture texture() const;

    private:
        model_texture   m_texture{0};
        raw_model       m_raw_model;
    };
}
