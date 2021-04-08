#pragma once

#include <memory>

class _ShowBase_Impl;

class ShowBase
{
public:
	void run();
	ShowBase();
	~ShowBase(); // need to define it in cpp otherwise it would try to generate inline destructor for unique_ptr and require ShowBase_Impl implementation

	ShowBase(const ShowBase&) = delete;
	ShowBase& operator= (const ShowBase&) = delete;
	ShowBase(ShowBase&&) = delete;
	ShowBase& operator= (ShowBase&&) = delete;

private:
	std::unique_ptr<_ShowBase_Impl> p_impl;
};