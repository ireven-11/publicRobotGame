#include"DxLib.h"
#include"ActionBase.h"
#include"RightAttack.h"
#include"PlayerStatus.h"
#include"IWeapon.h"
#include"InputHandler.h"
#include"RightAttackEvent.h"

RightAttack::RightAttack()
{
}

RightAttack::~RightAttack()
{
}

void RightAttack::update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)
{
	std::shared_ptr<PlayerStatus> playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status);

	if (canUpdate)
	{
		enter(status);
		
		if (InputHandler::instance().getRightAttackEvent()->getStartFiringGunTrigger()())
		{
			playerStatus->getRightWeapon()->update();
		}
	}
	else
	{
		exit(status);
	}
}