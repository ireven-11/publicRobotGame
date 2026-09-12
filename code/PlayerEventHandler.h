#pragma once
#include<memory>
#include"Singleton.h"

class LevelUpEvent;
class OnDamageEvent;

class PlayerEventHandler : public Singleton<PlayerEventHandler>
{
public:
	~PlayerEventHandler();

	const std::shared_ptr<LevelUpEvent> getLevelUpEvent()const noexcept { return levelUpEvent_; }
	const std::shared_ptr<OnDamageEvent> getOnDamageEvent()const noexcept { return onDamageEvent_; }

private:
	friend class Singleton<PlayerEventHandler>;
	PlayerEventHandler();

	std::shared_ptr<LevelUpEvent> levelUpEvent_;
	std::shared_ptr<OnDamageEvent> onDamageEvent_;
};