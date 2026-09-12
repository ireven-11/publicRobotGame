#pragma once
#include"IEvent.h"
#include<vector>

class StageOverEvent : public IEvent
{
public:
	StageOverEvent();
	~StageOverEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;
};