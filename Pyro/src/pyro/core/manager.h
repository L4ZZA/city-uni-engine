#pragma once

namespace pyro 
{

	class Manager
	{
	public:
		virtual void init() = 0;
		virtual void shutdown() = 0;

		virtual ~Manager() {}
	};
}