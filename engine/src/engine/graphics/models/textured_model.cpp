#include "pch.h"
#include "textured_model.h"

engine::textured_model::textured_model(const raw_model &model, const model_texture &texture)
{
	create(model, texture);
}

void engine::textured_model::create(const raw_model &model, const model_texture &texture)
{
	m_raw_model = model;
	m_texture = texture;
}

engine::raw_model engine::textured_model::raw() const
{
	return m_raw_model;
}

engine::model_texture engine::textured_model::texture() const
{
	return m_texture;
}
