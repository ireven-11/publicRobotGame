#pragma once
#include"IAnimation.h"
#include<vector>

class HoldUpRightGunAnim;
class HoldUpLeftGun;
class PlayerStatus;
class SlideMoveAnim;

class PlayerAnimator
{
public:
	PlayerAnimator(const std::shared_ptr<PlayerStatus>& statusPointer);
	~PlayerAnimator();

	void init();
	void update();
	
private:
	std::vector<std::shared_ptr<IAnimation<PlayerStatus>>> animations_;
	std::shared_ptr<PlayerStatus> playerStatus_;
};