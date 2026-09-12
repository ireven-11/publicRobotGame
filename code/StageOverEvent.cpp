#include"DxLib.h"
#include"StageOverEvent.h"

StageOverEvent::StageOverEvent()
{
}

StageOverEvent::~StageOverEvent()
{
	clearEvent();
}


void StageOverEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void StageOverEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void StageOverEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void StageOverEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void StageOverEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void StageOverEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void StageOverEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}