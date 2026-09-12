#include"DxLib.h"
#include"ActionBase.h"
#include"LeftAttack.h"
#include"PlayerStatus.h"
#include"IWeapon.h"
#include"InputHandler.h"
#include"LeftAttackEvent.h"

LeftAttack::LeftAttack()
{
}

LeftAttack::~LeftAttack()
{
}

void LeftAttack::update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)
{
	std::shared_ptr<PlayerStatus> playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status);

	if (canUpdate)
	{
		enter(status);

		//アニメーションの腕が上がりきる前は弾をうたないようにする
		if (InputHandler::instance().getLeftAttackEvent()->getStartFiringGunTrigger()())
		{
			playerStatus->getLeftWeapon()->update();
		}
	}
	else
	{
		exit(status);
	}
}