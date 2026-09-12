#pragma once
#include"IEvent.h"
#include<vector>
#include<functional>

class WalkEvent : public IEvent
{
public:
	WalkEvent();
	~WalkEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

	const std::function<double()>	getStickAngleTrigger()const noexcept { return stickAngleTrigger_; }
	const std::function<int()>		getStickXTrigger()const noexcept { return stickXTrigger_; }
	const std::function<int()>		getStickYTrigger()const noexcept { return stickYTrigger_; }
	const std::function<bool()>		getStartWalkingTrigger()const noexcept { return startWalkingTrigger_; }
	const std::function<bool()>		getWalkingTrigger()const noexcept { return walkingTrigger_; }
	const std::function<bool()>		getFinishWalkingTrigger()const noexcept { return finishWalkingTrigger_; }
	
private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;

	std::function<double()> stickAngleTrigger_;
	std::function<int()> stickXTrigger_;
	std::function<int()> stickYTrigger_;
	std::function<bool()> startWalkingTrigger_;
	std::function<bool()> walkingTrigger_;
	std::function<bool()> finishWalkingTrigger_;
};