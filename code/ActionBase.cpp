#include"DxLib.h"
#include"ActionBase.h"

ActionBase::ActionBase()
{
	init();
}
 
ActionBase::~ActionBase()
{

}

void ActionBase::init()
{
	canEnter_	= true;
	canExit_	= false;
}

void ActionBase::enter(const std::shared_ptr<StatusBase3D>& status)
{
	if (!canEnter_) return;

	canEnter_	= false;
	canExit_	= true;
}

void ActionBase::exit(const std::shared_ptr<StatusBase3D>& status)
{
	if (!canExit_) return;

	canEnter_	= true;
	canExit_	= false;
}