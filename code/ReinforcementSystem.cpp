#include"DxLib.h"
#include"PlayerStatus.h"
#include"BattleSoundManager.h"
#include"ReinforcementSystem.h"
#include"IWeapon.h"
#include"WeaponStatus.h"
#include"PlayerEventHandler.h"
#include"LevelUpEvent.h"
#include"InputHandler.h"
#include"DInput.h"
#include"Cannon.h"
#include"BombBug.h"

ReinforcementSystem::ReinforcementSystem(const std::shared_ptr<PlayerStatus>& playerStatus)
{
	const auto rightWeapon			= playerStatus->getRightWeapon();
	const auto leftWeapon			= playerStatus->getLeftWeapon();
	const auto rightWeaponStatus	= rightWeapon->getWeaponStatus();
	const auto leftWeaponStatus		= leftWeapon->getWeaponStatus();

	reinforce_[ReinforcingType::LEFT_ATTACK_POWER]	= [leftWeaponStatus]() {leftWeaponStatus->reinforceAttackPower(); };
	reinforce_[ReinforcingType::LEFT_ATTACK_RANGE]	= [leftWeaponStatus]() {leftWeaponStatus->reinforceAttackRange(); };
	reinforce_[ReinforcingType::LEFT_FIRE_RATE]		= [leftWeaponStatus]() {leftWeaponStatus->reinforceFireRate(); };
	reinforce_[ReinforcingType::LEFT_FIRE_SPEED]	= [leftWeaponStatus]() {leftWeaponStatus->reinforceFireBulletSpeed(); };

	reinforce_[ReinforcingType::LEFT_CHANGE_TO_BOMBBUG] = [playerStatus]() {playerStatus->reinforceOrChangeLeftWeapon<BombBug>(); };
	reinforce_[ReinforcingType::LEFT_CHANGE_TO_CANNON]	= [playerStatus]() {playerStatus->reinforceOrChangeLeftWeapon<Cannon>(); };

	reinforce_[ReinforcingType::RIGHT_ATTACK_POWER]	= [rightWeaponStatus]() {rightWeaponStatus->reinforceAttackPower(); };
	reinforce_[ReinforcingType::RIGHT_ATTACK_RANGE]	= [rightWeaponStatus]() {rightWeaponStatus->reinforceAttackRange(); };
	reinforce_[ReinforcingType::RIGHT_FIRE_RATE]	= [rightWeaponStatus]() {rightWeaponStatus->reinforceFireRate(); };
	reinforce_[ReinforcingType::RIGHT_FIRE_SPEED]	= [rightWeaponStatus]() {rightWeaponStatus->reinforceFireBulletSpeed(); };

	reinforce_[ReinforcingType::RIGHT_CHANGE_TO_BOMBBUG] = [playerStatus]() {playerStatus->reinforceOrChangeRightWeapon<BombBug>(); };
	reinforce_[ReinforcingType::RIGHT_CHANGE_TO_CANNON] = [playerStatus]() {playerStatus->reinforceOrChangeRightWeapon<Cannon>(); };

	//イベントを登録
	PlayerEventHandler::instance().getLevelUpEvent()->addStartingEvent([this]() {return start(); });

	init();
}

ReinforcementSystem::~ReinforcementSystem()
{
}

void ReinforcementSystem::init()
{
	isActive_		= false;
	selectorIndex_	= 0;
}

void ReinforcementSystem::update(const std::shared_ptr<BattleSoundManager>& sound)
{
	if (!isActive_) return;

	PlayerEventHandler::instance().getLevelUpEvent()->occurPlayingEvent();

	//1回だけ強化内容候補を選ぶ
	choiceReinforcingOptions();

	//強化内容を選択する
	selectReinforcement(sound);

	//教科内容を決定する
	decideReinforcement(sound);
}

void ReinforcementSystem::start()
{
	isActive_ = true;

	//強化内容候補が最大数より増えないようにする
	reinforcingOptions_.clear();
	choicedReinforcement_.clear();
}

void ReinforcementSystem::choiceReinforcingOptions()
{
	if (!reinforcingOptions_.empty()) return;

	//回数分、ランダムで強化内容候補を決定
	for (int i = 0; i < max_options_number; i++)
	{
		const auto random = GetRand(static_cast<int>(ReinforcingType::COUNT) - 1);	//COUNTが選ばれないように-1してる

		reinforcingOptions_.emplace_back(reinforce_[static_cast<ReinforcingType>(random)]);
		choicedReinforcement_.emplace_back(static_cast<ReinforcingType>(random));
	}
}

void ReinforcementSystem::selectReinforcement(const std::shared_ptr<BattleSoundManager>& sound)
{
	//スティックを倒してなかったら処理をしない
	if (!InputHandler::instance().getDInput()->getStickTiledMoment(PROCON::STICK::LEFT_X)) return;

	//スティックを右に倒したとき
	if (InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_X) > 0)
	{
		++selectorIndex_;

		if (selectorIndex_ >= max_options_number)
		{
			selectorIndex_ = 0;
		}
	}
	else if (InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_X) < 0)
	{
		--selectorIndex_;

		if (selectorIndex_ < 0)
		{
			selectorIndex_ = max_options_number - 1;
		}
	}

	sound->start2DSound("selectReinforcement", false, true);
}

void ReinforcementSystem::decideReinforcement(const std::shared_ptr<BattleSoundManager>& sound)
{
	if (InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::A))
	{
		reinforcingOptions_[selectorIndex_]();
		isActive_ = false;
		PlayerEventHandler::instance().getLevelUpEvent()->occurFinishingEvent();
	}
	else if (InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::B))
	{
		//何も選択しない
		isActive_ = false;
		PlayerEventHandler::instance().getLevelUpEvent()->occurFinishingEvent();
	}

	if (isActive_) return;

	sound->start2DSound("decideReinforcement");
}