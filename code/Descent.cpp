#include"DxLib.h"
#include"ActionBase.h"
#include"PlayerStatus.h"
#include"Descent.h"

Descent::Descent()
{
}

Descent::~Descent()
{
}

void Descent::update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)
{
	std::shared_ptr<PlayerStatus> playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status);

	if (canUpdate)
	{
		enter(status);
		
		playerStatus->descentPosition();
	}
	else
	{
		exit(status);
	}
}