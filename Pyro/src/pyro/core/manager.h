#pragma once

namespace pyro 
{

	class Manager
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual ~Manager() {}
	};
}