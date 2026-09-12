#pragma once
#include"IEvent.h"
#include<vector>
#include<functional>

class CameraEvent : public IEvent
{
public:
	CameraEvent();
	~CameraEvent();

	void addStartingEvent(const std::function<void()>& event);
	void addFinishingEvent(const std::function<void()>& event);
	void addPlayingEvent(const std::function<void()>& event);
	void occurStartingEvent();
	void occurFinishingEvent();
	void occurPlayingEvent();
	void clearEvent();

	const std::function<int()>	getStickXTrigger()const noexcept { return stickXTrigger_; }
	const std::function<int()>	getStickYTrigger()const noexcept { return stickYTrigger_; }

private:
	std::vector<std::function<void()>> startingEvents_;
	std::vector<std::function<void()>> finishingEvents_;
	std::vector<std::function<void()>> playingEvents_;

	std::function<int()> stickXTrigger_;
	std::function<int()> stickYTrigger_;
};