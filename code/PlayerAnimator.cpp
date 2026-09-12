#include"DxLib.h"
#include<memory>
#include"HoldUpRightGunAnim.h"
#include"HoldUpLeftGunAnim.h"
#include"SlideMoveAnim.h"
#include"PlayerAnimator.h"

PlayerAnimator::PlayerAnimator(const std::shared_ptr<PlayerStatus>& statusPointer)
	:playerStatus_(statusPointer)
{
	animations_.emplace_back(std::make_shared<HoldUpRightGunAnim>());
	animations_.emplace_back(std::make_shared<HoldUpLeftGunAnim>());
	animations_.emplace_back(std::make_shared<SlideMoveAnim>());
}

PlayerAnimator::~PlayerAnimator()
{
	playerStatus_ = nullptr;
	animations_.clear();
}

void PlayerAnimator::init()
{
	for (const auto& anim : animations_)
	{
		anim->init();
	}
}

void PlayerAnimator::update()
{
	for (const auto& anim : animations_)
	{
		anim->update(playerStatus_);
	}
}