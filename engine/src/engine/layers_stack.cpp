#include "pch.h"
#include "layers_stack.h"

engine::layers_stack::layers_stack()
{}

engine::layers_stack::~layers_stack()
{
	for(auto* layer : m_layers)
		delete layer;
}

void engine::layers_stack::push_layer(layer* layer)
{
	m_layers.emplace(m_layers.begin() + m_layers_insert_index, layer);
	m_layers_insert_index++;
}

void engine::layers_stack::push_overlay(layer* overlay)
{
	m_layers.emplace_back(overlay);
}

void engine::layers_stack::pop_layer(layer* layer)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if(it != m_layers.end())
	{
		m_layers.erase(it);
		--m_layers_insert_index;
	}
}

void engine::layers_stack::pop_overlay(layer* overlay)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
	if(it != m_layers.end())
		m_layers.erase(it);
}
