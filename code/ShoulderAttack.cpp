#include"DxLib.h"
#include"ActionBase.h"
#include"ShoulderAttack.h"
#include"IWeapon.h"
#include"PlayerStatus.h"

ShoulderAttack::ShoulderAttack()
{
}

ShoulderAttack::~ShoulderAttack()
{
}

void ShoulderAttack::update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)
{
	std::shared_ptr<PlayerStatus> playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status);

	if (canUpdate)
	{
		enter(status);

		playerStatus->getShoulderWeapon()->update();
	}
	else
	{
		exit(status);
	}
}