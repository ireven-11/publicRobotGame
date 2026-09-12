#pragma once
#include"CharaBase.h"

class InputHandler;
class PlayerAnimator;
class PlayerAction;
class PlayerStatus;

class Player : public CharaBase
{
public:
	Player();
	virtual ~Player();

	void init();
	void update();
	void draw()const;

	const std::shared_ptr<PlayerAction> getPlayerAction()const noexcept { return action_; }

private:
	void action();
	void modelRotation();
	void event();

	std::shared_ptr<PlayerAnimator>		animator_;
	std::shared_ptr<PlayerAction>		action_;
};