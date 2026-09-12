#include"DxLib.h"
#include"Dash.h"
#include"Walk.h"
#include"descent.h"
#include"raise.h"
#include"RightAttack.h"
#include"LeftAttack.h"
#include"ShoulderAttack.h"
#include"Interact.h"
#include"PlayerStatus.h"
#include"PlayerAction.h"

PlayerAction::PlayerAction()
	: dash_(std::make_shared<Dash>()),
	walk_(std::make_shared<Walk>()),
	descent_(std::make_shared<Descent>()),
	raise_(std::make_shared<Raise>()),
	rightAttack_(std::make_shared<RightAttack>()),
	leftAttack_(std::make_shared<LeftAttack>()),
	shoulderAttack_(std::make_shared<ShoulderAttack>()),
	interact_(std::make_shared<Interact>())
{
	init();
}

PlayerAction::~PlayerAction()
{
}

void PlayerAction::init()
{
	isDash_ = false;
	isWalk_ = false;
}

void PlayerAction::dash(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	isDash_ = canUpdate;
	if (isDash_) isWalk_ = false;

	dash_->update(status, isDash_);
}

void PlayerAction::walk(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	isWalk_ = canUpdate && !isDash_;

	walk_->update(status, isWalk_);
}

void PlayerAction::descent(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	descent_->update(status, canUpdate);
}

void PlayerAction::raise(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	raise_->update(status, canUpdate);
}

void PlayerAction::rightAttack(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	rightAttack_->update(status, canUpdate);
}

void PlayerAction::leftAttack(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	leftAttack_->update(status, canUpdate);
}

void PlayerAction::shoulderAttack(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	shoulderAttack_->update(status, canUpdate);
}

void PlayerAction::interact(const std::shared_ptr<CharaStatus>& status, const bool canUpdate)
{
	interact_->update(status, canUpdate);
}