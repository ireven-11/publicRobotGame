#pragma once
#include"IEvent.h"
#include<vector>
#include<functional>

class RaiseEvent : public IEvent
{
public:
	RaiseEvent();
	~RaiseEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

	const std::function<bool()>	getRaiseTrigger()const noexcept { return raiseTrigger_; }

private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;

	std::function<bool()> raiseTrigger_;
	std::function<bool()> startRaiseTrigger_;
	std::function<bool()> finishRaiseTrigger_;
};