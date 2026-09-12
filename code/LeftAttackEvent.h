#pragma once
#include"IEvent.h"
#include<vector>

class LeftAttackEvent : public IEvent
{
public:
	LeftAttackEvent();
	~LeftAttackEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

	const std::function<bool()> getLeftAttackingTrigger()const noexcept { return leftAttackingTrigger_; }
	const std::function<bool()> getStartFiringGunTrigger()const noexcept { return startFiringGunTrigger_; }

private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;

	std::function<bool()> leftAttackingTrigger_;
	std::function<bool()> startFiringGunTrigger_;

	const float one_seconds_dxlib = 1000.0f;
};