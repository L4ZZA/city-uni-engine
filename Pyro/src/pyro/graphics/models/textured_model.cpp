#include "pyro_pch.h"
#include "textured_model.h"

pyro::textured_model::textured_model(const raw_model &model, const model_texture &texture)
{
	create(model, texture);
}

void pyro::textured_model::create(const raw_model &model, const model_texture &texture)
{
	m_raw_model = model;
	m_texture = texture;
}

pyro::raw_model pyro::textured_model::raw() const
{
	return m_raw_model;
}

pyro::model_texture pyro::textured_model::texture() const
{
	return m_texture;
}
