#pragma once
#include"IAction.h"

class ActionBase : public IAction
{
public:
	ActionBase();
	~ActionBase();

	virtual void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate) = 0;

protected:
	void init();
	void enter(const std::shared_ptr<StatusBase3D>& status);
	void exit(const std::shared_ptr<StatusBase3D>& status);

	bool canEnter_;
	bool canExit_;
};