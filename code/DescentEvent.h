#pragma once
#include"IEvent.h"
#include<vector>

class DescentEvent : public IEvent
{
public:
	DescentEvent();
	~DescentEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

	const std::function<bool()>	getDescentTrigger()const noexcept { return descentTrigger_; }

private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;

	std::function<bool()> descentTrigger_;
	std::function<bool()> startDescentTrigger_;
	std::function<bool()> finishDescentTrigger_;
};