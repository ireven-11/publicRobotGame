#pragma once
#include"IEvent.h"
#include<vector>
#include<functional>

class DashEvent : public IEvent
{
public:
	DashEvent();
	~DashEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

	const std::function<bool()>	getDashTrigger()const noexcept { return dashTrigger_; }
	const std::function<bool()>	getStartDashTrigger()const noexcept { return startDashTrigger_; }
	const std::function<bool()>	getFinishDashTrigger()const noexcept { return finishDashTrigger_; }

private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;

	std::function<bool()> dashTrigger_;
	std::function<bool()> startDashTrigger_;
	std::function<bool()> finishDashTrigger_;
};