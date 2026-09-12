#pragma once
#include"StatusBase3D.h"
#include<memory>

class IAction
{
public:
	virtual ~IAction() = default;

	virtual void enter(const std::shared_ptr<StatusBase3D>& status)							= 0;
	virtual void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)	= 0;
	virtual void exit(const std::shared_ptr<StatusBase3D>& status)							= 0;
};